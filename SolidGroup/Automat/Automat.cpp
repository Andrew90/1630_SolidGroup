#include "stdafx.h"
#include "Automat.h"
#include <Windows.h>
#include "l502SolidGroup.h"
#include "AppBase.h"
#include "TopLabelViewer.h"
#include "SolidData.h"
#include "Compute.h"
#include "Corel.h"
#include "StoredData.h"
#include "MainWindow.h"
#include "SaveLoadDates.h"
#include "ChangeOut.h"
#include "SolidCounter.h"
#include "DebugMess.h"

namespace Automat
{	
	L502SolidGroup &x502 = Singleton<L502SolidGroup>::Instance(); 
	HANDLE hThread, hStart, hStop;
	void(*dataChanged)(int);

	unsigned &MODULE_ON = (unsigned &)Singleton<DInputsParametersTable>::Instance().items.get<ModuleOn>().value;
	unsigned &TUBE_IN   = (unsigned &)Singleton<DInputsParametersTable>::Instance().items.get<TubeIn>().value;

	bool stop;

	DWORD WINAPI __Do__(LPVOID);

	void Crock(int){};

	void Init()
	{
		dataChanged = Crock;
		stop = true;
		hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
		hThread = CreateThread(NULL, 0, __Do__, NULL, 0, NULL);
	}

	void Destroy()
	{
		SuspendThread(hThread);
		CloseHandle(hThread);
		CloseHandle(hStart);
	}

	void Start()
	{
		SetEvent(hStart);
	}

	void Stop()
	{
		stop = true;
		ResetEvent(hStart);
	}

	bool UserStop()
	{
		if(stop)
		{
			topLabelViewer.SetMessage(L"<ff>Оператор вышел из режима сбора данных");
			x502.Stop();
		}
		return stop;
	}

	DWORD WINAPI __Do__(LPVOID)
	{
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		while(true)
		{
START:
			mainWindow.BottomLabel(0, L"");
			WaitForSingleObject(hStart, INFINITE);
			if(stop)
			{
				stop = false;
				x502.Start();
				topLabelViewer.SetMessage(L"<ff>Установка запущена");
			}
			unsigned startChennel = 0;
			unsigned count; 
			unsigned input;
			double data[1024];

			mainWindow.BottomLabel(0, L"Ожидание трубы");

			while(true)
			{
				count = dimention_of(data);
				input = 0;
				x502.Read(startChennel, data, count, input);

				if(0 == (input & MODULE_ON))
				{
					topLabelViewer.SetMessage(L"<ff0000>Нет готовности установки");
					Stop();
					x502.Stop();					
					goto START;
				}

				if(input & TUBE_IN)
				{
					solidData.Clear();
					topLabelViewer.SetMessage(L"<ff>Труба в модуле");					
					break;
				}

				if(UserStop()) goto START;
			}

			mainWindow.BottomLabel(0, L"Сбор данных");
			mainWindow.colorPanel.Clear();

			while(true)
			{
				count = dimention_of(data);
				input = 0;
				x502.Read(startChennel, data, count, input);

				if(0 == (input & MODULE_ON))
				{
					topLabelViewer.SetMessage(L"<ff0000>Нет готовности установки");
					Stop();
					x502.Stop();					
					goto START;
				}

				if(input & TUBE_IN)
				{
					solidData.SetData(data, count, startChennel);
					dprint("count samples %d", count);
					(*dataChanged)(1000);
				}
				else
				{
					compute.Do();
					(*dataChanged)(0);

					wchar_t buf[1024] = L"";

					if(0 != corel.inputItem.classTube && corel.classTubeItem[corel.inputItem.classTube])
					{
						Corel::ClassTubeItem *tube	= corel.classTubeItem[corel.inputItem.classTube];
						wchar_t *sgName = (wchar_t *)tube->Name.c_str();

						SolidCounter sg;
						wchar_t *typeSize = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
						sg.Load(typeSize);

						int count = sg.SolidGroupCount(sgName) + 1;

						sg.Set(count, typeSize, sgName);
						mainWindow.gridCounter.Update();

						StoredData::Store(typeSize, sgName);

						wsprintf(buf,  L"<ff>Группа прочности <%8x>%s <ff>%s"
							, tube->color
							, sgName
							,  Wchar_from<double>(corel.inputItem.correlation)()
							);	

						mainWindow.colorPanel.SetText(
							(wchar_t *)corel.classTubeItem[corel.inputItem.classTube]->Name.c_str()
							, corel.classTubeItem[corel.inputItem.classTube]->color
							);
					}

					topLabelViewer.SetMessage(buf);

					if(BST_CHECKED == Button_GetCheck(mainWindow.hStoredCheckBox))
					{
						StoreDataFile();
					}					
					goto START;
				}

				if(UserStop()) goto START;
			}
		}
	}
}