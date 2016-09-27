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

bool Compute::SubCompute(int(&tresholds)[8], int start, int stop, Data &data, double(&inputs)[1024])
{
	std::map<int, TItem> mapItems;	
	double length = frequency502 / (2 * 2 * frequenctGenerator);
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
		while(data.referenceBuffer(i, sample) && sample < 0 && i < stop) ++i;
		int k = 0;
		int zeroStart = i;
		bool b;
		for(; k < dimention_of(inputs)&&(b = data.referenceBuffer(i, sample)) && sample > 0; ++k, ++i);
		if(!b || i == start) break;
		for(int z = 0; z < k && data.dataBuffer(z + zeroStart, sample); ++z)
		{
			inputs[z] += sample;
		}
		++count;
		for(int z = 0; z < dimention_of(corel.inputItem.elements) && data.dataBuffer(zeroStart + offs[z], sample); ++z)
		{
			corel.inputItem.elements[z] = sample;
		}
		corel.Compute();
		++mapItems[corel.inputItem.classTube].count;
		mapItems[corel.inputItem.classTube].correlation += corel.inputItem.correlation;
	}
	if(count > 0) 
	{
		for(int i = 0; i < dimention_of(corel.inputItem.elements); ++i)
		{
			corel.inputItem.elements[i] = inputs[offs[i]] / count;
		}		
	}
	int classTube = 0;
	double correlation = 0;
	for_each(mapItems.begin(), mapItems.end(), MapMax(classTube, correlation));
	corel.inputItem.classTube = classTube;
	corel.inputItem.correlation = correlation;
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
	double inputs[1024] = {};
	corel.inputItem.classTube = 0;
	SubCompute(
		tresholds
		, solidData.start
		, solidData.stop
		, DataBufferXX()
		, inputs
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
		s += corel.classTubeItem[corel.inputItem.classTube]->Name;	
		s += L" ";
		s += Wchar_from<double>(corel.inputItem.correlation)();
	}
	else
	{
		s = L"";
	}
	topLabelViewer.SetMessage((wchar_t *)s.c_str());
}