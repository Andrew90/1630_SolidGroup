#include "stdafx.h"
#include "Compute.h"
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

struct DataBuffer: Compute::Data
{
	double dataBuffer(int i)
	{
		return solidData.dataBuffer[i];
	}
	double referenceBuffer(int i)
	{
		return solidData.referenceBuffer[i];
	}
};

bool Compute::SubCompute(int(&tresholds)[8], int start, int stop, Data &data, double(&inputs)[1024], int &length)
{
#pragma message("дописанны???? настройки управления частоты платы и частоту токового сигнала")
	const int period = frequency502 / (2 * 2 * frequenctGenerator);
	const int periodMin = int(0.9 * period);
	const int perionMax = int(1.1 * period);
	int count = 0;
	length = 0;
	for(int i = start; i < stop;)
	{
		while(data.referenceBuffer(i) > 0 && i < stop) ++i;
		int k = 0;
		int zeroStart = i;
		
		for(; k < dimention_of(inputs)&&data.referenceBuffer(i) < 0; ++k, ++i);
	
		if(k > periodMin && k < perionMax)
		{
			for(int z = 0; z < k; ++z)
			{
				inputs[z] += data.dataBuffer(z + zeroStart);
			}
			++count;
		dprint("count__ %d %d", k, count);
		length += k;
		}
	}
	if(count > 0) 
	{
		dprint("length %d", length);
		for(int i = 0; i < dimention_of(corel.inputItem.elements); ++i)
		{
			int z = int((double)period * tresholds[i] / 100.0);
			
			corel.inputItem.elements[i] = inputs[z] / count;
			dprint("offsets__ %d %f", z, corel.inputItem.elements[i]);
		}
		length = period;
		corel.Compute();
		return true;
	}
	return false;
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
	 int length;
    if(SubCompute(
		tresholds
		, solidData.start
		, solidData.stop
		, DataBuffer()
		, inputs
		, length
		)
	)
	{
		if(0 != corel.inputItem.classTube)
		{
			CommunicationTCP::Result = corel.classTubeItem[corel.inputItem.classTube]->communicationID;
		}
		else
		{
			CommunicationTCP::Result = 0;
		}
		return;
	}
	corel.inputItem.classTube = 0;
}
void Compute::Recalculation()
 {
	Do();
	CommunicationTCP::Result = 0;
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