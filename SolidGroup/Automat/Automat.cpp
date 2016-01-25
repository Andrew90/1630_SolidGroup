#include "stdafx.h"
#include "Automat.h"
#include "Device1730.h"
#include <string>
#include "l502SolidGroup.h"
#include "SolidData.h"
#include "Compute.h"
#include "MainWindow.h"
#include "Corel.h"
#include "ClientTreshold.h"
#include "MainWindow.h"
#include "CounterTubes.h"
#include "ChangeOut.h"
#include "StoredData.h"
#include "SaveLoadDates.h"
#include "DebugMess.h"
#include "AdditionalBase.h"
//-------------------------------------------
static const unsigned UNIT_ON = 2;
static const unsigned TUBE_IN_UNIT = 1;
void (*ptr)(unsigned);
void AlarmStep(unsigned input);
void ReadDate(unsigned);
void ComputeStep(unsigned );

void StartStep(unsigned input)
{
	if(UNIT_ON == (input & UNIT_ON))
	{
		if(TUBE_IN_UNIT  == (input & TUBE_IN_UNIT))
		{
			topLabelViewer.SetMessage(L"<ff0000>Сбор данных");
			solidData.Clear();
			l502SolidGroup.Start();
			ptr = ReadDate;
			CommunicationTCP::Result = 0;
			if(Singleton<AdditionalParametersTable>::Instance().items.get<MarkPaint>().value)
			{
				device1730.Write(0);
			}
		}
	}
	else
	{
		topLabelViewer.SetMessage(L"<ff0000>Нет готовности установки");
		ptr = AlarmStep;
	}
}
void Corck(){}
void (*Automat::dataChanged)() = Corck;
void ReadDate(unsigned input)
{
	if(UNIT_ON == (input & UNIT_ON))
	{
		if(TUBE_IN_UNIT == (input & TUBE_IN_UNIT))
		{
			unsigned first = 0;
			double data[1024];
			unsigned res = 1024;
			l502SolidGroup.Read(first, data, res);
			solidData.SetData(&data[first], res - first);
			static unsigned lastTime = 0;
			unsigned currentTime = GetTickCount();
			if(currentTime - lastTime > 5000)
			{
				lastTime = currentTime;
				(*Automat::dataChanged)();
			}
		}
		else
		{
			l502SolidGroup.Stop();
			ptr = ComputeStep;
		}
	}
	else
	{
		l502SolidGroup.Stop();
		ptr = StartStep;
	}
}

void ComputeStep(unsigned )
{
	l502SolidGroup.Stop();	
	compute.Do();
	int communicationID = 0;
	if(0 != corel.inputItem.classTube && corel.classTubeItem[corel.inputItem.classTube])
	{
		int tmpCommunication = corel.classTubeItem[corel.inputItem.classTube]->communicationID;
		if(changeOut.toOut.size() && changeOut.toOut[tmpCommunication])
		{
			communicationID = changeOut.toOut[tmpCommunication];
		}
		if(0 == communicationID)
		{
			communicationID = tmpCommunication;
		}
	}
	///
	///\todo здесь написать выход группы прочности на краскоотметчик
	if(Singleton<AdditionalParametersTable>::Instance().items.get<MarkPaint>().value && communicationID >= 10 && communicationID <= 15)
	{
		device1730.Write( 1 << communicationID);
	}
	///
	std::wstring s = L"<ff>Результат измерения ";
	wchar_t buff[128];
	s += _itow(StoredData::Instance().Current(), buff, 10);
	s += L". Группа прочности<ff0000>";
	if(communicationID)
	{
		for(std::map<int, Corel::ClassTubeItem *>::iterator i = corel.classTubeItem.begin(); i != corel.classTubeItem.end(); ++i)
		{
			if(i->second->communicationID == communicationID) 
			{
				s += i->second->Name;
				s += L" ";
				s += Wchar_from<double>(corel.inputItem.correlation)();
				
				StoredData::Instance().Store(
					Singleton<ParametersTable>::Instance().items.get<NameParam>().value
					, (wchar_t *)i->second->Name.c_str()
				);
				break;
			}
		}
	}
	topLabelViewer.SetMessage((wchar_t *)s.c_str());
    if(BST_CHECKED == Button_GetCheck(mainWindow.hStoredCheckBox))
	{
		StoreDataFile();
	}
	ptr = StartStep;
	(*Automat::dataChanged)();
	StoredData::Instance().Inc();
}

void AlarmStep(unsigned input)
{
	if(UNIT_ON == (input & UNIT_ON) || automat.start) 
	{
			ptr = StartStep;
	}
}

void Automat::Do(Automat *self)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    self->contine = true;
	ptr = StartStep;
	while(self->contine)
	{
		WaitForSingleObject(hEvent, 10);
		if(self->start)
		{
			unsigned input = device1730.Read();
			(*ptr)(input);
		}
		else
		{
			Sleep(1000);
		}
	}
	CloseHandle(hEvent);
}
//--------------------------------------
Automat::Automat()
{}
//------------------------------------------------
void Automat::Init()
{
	start = false;
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Automat::Do, this, 0, NULL));
}
//---------------------------------------------
void Automat::Destroy()
{
	contine = false;
}
//--------------------------------------------------
void Automat::Start()
{	
	if(0 != Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value)
	{
		if(!clientTreshold.Connect(
			Singleton<TcpCommunications>::Instance().items.get<PortTCP>().value
			, Singleton<TcpCommunications>::Instance().items.get<AddresTCP>().value
			))
		{
			MessageBox(0, L"Не могу подключится к рабочей станции", L"Ошибка !!!", MB_ICONERROR);
			Button_SetCheck(mainWindow.hCommunicationCheckBox, BST_UNCHECKED);
			return;
		}
	}
	EnableWindow(mainWindow.hCommunicationCheckBox, false);
	start = true;
	wchar_t * s = L"<ff>Установка запущена";
	if(changeOut())s = L"<ff>Установка запущена.";
	topLabelViewer.SetMessage(s);
}
//-------------------------------------------------------
void Automat::Stop()
{
	start = false;
	topLabelViewer.SetMessage(L"<ff>Установка остановлена");	
	clientTreshold.Close();
	EnableWindow(mainWindow.hCommunicationCheckBox, true);
}
//-----------------------------------------------------------