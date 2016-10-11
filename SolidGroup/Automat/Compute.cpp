#include "stdafx.h"
#include "Compute.h"
#include <algorithm>
#include "SolidData.h"
#include "ChartData.h"
#include "AppBase.h"
#include "Corel.h"
#include "TopLabelViewer.h"
#include "Communication.h"
#include "SaveLoadDates.h"
#include "Templates.hpp"
#include "DebugMess.h"
#include "MainWindow.h"


Compute::Compute()
	: frequency502(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
	, frequenctGenerator(Singleton<SolenoidParametersTable>::Instance().items.get<FrequencyGenerator>().value)
{
}

template<class O, class P>struct __tresholds__
{
	void operator()(O *o, P *p)
	{
		static const int i = TL::IndexOf<ThresholdsTable::items_list, O>::value;
		p[i] = o->value;
	}
};

namespace
{
	struct TItem
	{
		int count;
		double correlation;
	};
	typedef std::map<int, TItem>::value_type TMapItem; 
	struct MapMax
	{
		int &classTube, count;
		double &correlation;
		MapMax(int &classTube, double &correlation)
			: classTube(classTube)
			, count(0)
			, correlation(correlation)
		{}
		void operator()(TMapItem &x)
		{
			if(x.second.count > count)
			{
				classTube = x.first;
				count = x.second.count;
				if(count > 0)correlation = x.second.correlation / count;
			}
		}
	};
}

DataBufferXX::DataBufferXX()
	: ptr(&DataBufferXX::ref)
{
	if(Singleton<SyncroDataTable>::Instance().items.get<SyncroData>().value)
	{
		ptr = &DataBufferXX::dataBuffer;
	}
}
bool DataBufferXX::dataBuffer(int i, double &d)
{
	d = solidData.dataBuffer[i];
	return true;
}
bool DataBufferXX::ref(int i, double &d)
{
	d = solidData.referenceBuffer[i];
	return true;
}
bool DataBufferXX::referenceBuffer(int i, double &d)
{
	return (this->*ptr)(i, d);
}

bool Compute::SubCompute(int(&tresholds)[8], int start, int stop, Data &data)
{
	//memset(inputs, 0, sizeof(inputs));
	double inputs[8] = {};
	std::map<int, TItem> mapItems;	
	double length = frequency502 / (2 * 2 * frequenctGenerator);
	int minL = int(0.9 * length);
	int maxL = int(1.1 * length);
	int count = 0;
	int first = start;
	int offs[dimention_of(tresholds)];
	for(int i = 0; i < dimention_of(offs); ++i)
	{
		offs[i] = int((double)length * tresholds[i] / 100.0);
	}

	double sample;
	for(int i = first; i < stop;)
	{
		int start = i;
		while(data.referenceBuffer(i, sample) && sample > 0 && i < stop) ++i;
		//int k = 0;
		int zeroStart = i;

		double y0;
		data.referenceBuffer(i - 1, y0);
		double y1;
		data.referenceBuffer(i, y1);

		double dY = y0/(y1 - y0);
		if(dY < 0) dY = -dY;
		bool b;
		for(;(b = data.referenceBuffer(i, sample)) && sample < 0;++i);
		if(!b || i == start) break;
		if(i > stop) break;
		int tLen = i - zeroStart;
		if(tLen < minL || tLen > maxL) continue;
		//	if(dY > 1.0) break;
		//for(int z = 0; z < k && data.dataBuffer(z + zeroStart, sample); ++z)
		//{
		//	data.dataBuffer(z + zeroStart - 1, y0);
		//	double y = y0 + dY * (sample - y0);
		//	inputs[z] += sample;
		//}
		//++count;
		for(int z = 0; z < dimention_of(corel.inputItem.elements) && data.dataBuffer(zeroStart + offs[z], sample); ++z)
		{
			data.dataBuffer(zeroStart + offs[z] - 1, y0);
			double y = y0 + dY * (sample - y0);
			corel.inputItem.elements[z] = y;
			inputs[z] += y;
		}
		++count;
		corel.Compute();
		++mapItems[corel.inputItem.classTube].count;
		mapItems[corel.inputItem.classTube].correlation += corel.inputItem.correlation;
	}
	if(count > 1) 
	{
		for(int i = 0; i < dimention_of(corel.inputItem.elements); ++i)
		{
			inputs[i] /= count;
		}		
	}
	int classTube = 0;
	double correlation = 0;
	for_each(mapItems.begin(), mapItems.end(), MapMax(classTube, correlation));
	corel.inputItem.classTube = classTube;
	corel.inputItem.correlation = correlation;
	memmove(corel.inputItem.elements, inputs, sizeof(corel.inputItem.elements));
	return true;
}

void Compute::Do()
{
	typedef SelectWithWapper<ThresholdsTable::items_list>::Result list;
	int tresholds[TL::Length<list>::value];
	double sum[TL::Length<list>::value] = {};
	TL::foreach<list, __tresholds__>()(
		&Singleton<ThresholdsTable>::Instance().items
		, tresholds
		);
	solidData.start = int(0.1 * solidData.currentOffset);
	solidData.stop = solidData.currentOffset - solidData.start;
	//double inputs[8] = {};
	corel.inputItem.classTube = 0;
	SubCompute(
		tresholds
		, solidData.start
		, solidData.stop
		, DataBufferXX()
		//, inputs
		);
}
void Compute::Recalculation()
{
	Do();
	std::wstring s = L"<ff0000>";
	s += LoadDateFile::path;
	s += L". <ff>Результат контроля. Группа прочности ";
	if(0 != corel.inputItem.classTube)
	{
		wchar_t buf[32];
		wsprintf(buf, L"<%8x>", corel.classTubeItem[corel.inputItem.classTube]->color);
		s += buf;
		s += corel.classTubeItem[corel.inputItem.classTube]->Name;	
		s += L" <ff>";
		s += Wchar_from<double>(corel.inputItem.correlation)();

		mainWindow.colorPanel.SetText(
		(wchar_t *)corel.classTubeItem[corel.inputItem.classTube]->Name.c_str()
		, corel.classTubeItem[corel.inputItem.classTube]->color
		);
	}
	else
	{
		s = L"";
		mainWindow.colorPanel.Clear();
	}
	topLabelViewer.SetMessage((wchar_t *)s.c_str());
	
}