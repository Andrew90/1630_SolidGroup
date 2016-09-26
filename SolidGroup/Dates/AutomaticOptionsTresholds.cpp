#include "stdafx.h"
#include "AutomaticOptionsTresholds.h"
#include "Compute.h"
#include "AppBase.h"
#include "SolidData.h"
#include "Corel.h"
#include "AutomaticThresholdsWindow.h"

#include "DebugMess.h"
//-----------------------------------------------------------------------------------------
namespace AutomaticOptionsTresholdsNameSpace
{
template<class O, class P>struct __tresholds__
{
	void operator()(O *o, P *p)
	{
		static const int i = TL::IndexOf<ThresholdsTable::items_list, O>::value;
		p[i] = o->value;
	}
};
template<class List>struct SelectWithWapper;
template<class Head, class Tail>struct SelectWithWapper<Tlst<Head, Tail> >
{
	typedef typename SelectWithWapper<Tail>::Result Result;
};
template<int N, template<int>class W, class Tail>struct SelectWithWapper<Tlst<W<N>, Tail> >
{
	typedef typename Tlst<W<N>, typename SelectWithWapper<Tail>::Result> Result;
};
template<>struct SelectWithWapper<NullType>
{
	typedef NullType Result;
};
}
//--------------------------------------------------------------------------------------
struct DataBufferX: Compute::Data
{
	FILE *f;
	int dataBufferOffsetMin, dataBufferOffsetMax;
	int referenceBufferOffsetMin, referenceBufferOffsetMax;
	int currentOffset;
	bool ok;
	double dataBuf[1024];
	double referenceBuf[1024];
	DataBufferX(wchar_t *path)
		: dataBufferOffsetMin(-1)
		, dataBufferOffsetMax(-1)
		, referenceBufferOffsetMin(-1)
		, referenceBufferOffsetMax(-1)
		, ok(false)
	{
		f = _wfopen(path, L"rb");
		if(f && fread(&currentOffset, sizeof(currentOffset), 1, f))
		{
			ok = true;
		}
	}
	~DataBufferX()
	{
		if(f)fclose(f);
	}
	bool dataBuffer(int i, double &d)
	{
		if(i > currentOffset) return false;
		if(ok && i > dataBufferOffsetMin && i < dataBufferOffsetMax)
		{
	    	d = dataBuf[i - dataBufferOffsetMin];
			return true;
		}
		else
		{
			ok = 0 == fseek(f, sizeof(currentOffset) + sizeof(double) * (currentOffset + i), SEEK_SET)
			&& 0 < fread(dataBuf, sizeof(dataBuf), 1, f);
			if(ok)
			{
				dataBufferOffsetMin = i;
				dataBufferOffsetMax = dataBufferOffsetMin + dimention_of(dataBuf);
				d = dataBuf[0];
				return true;
			}
		}
		return false;
	}
	bool referenceBuffer(int i, double &d)
	{
		if(i > currentOffset) return false;
		if(ok && i > referenceBufferOffsetMin && i < referenceBufferOffsetMax)
		{
	    	d = referenceBuf[i - referenceBufferOffsetMin];
			return true;
		}
		else
		{
			ok = 0 == fseek(f, sizeof(currentOffset) + sizeof(double) * i, SEEK_SET)
			&& 0 < fread(referenceBuf, sizeof(referenceBuf), 1, f);
			if(ok)
			{
				referenceBufferOffsetMin = i;
				referenceBufferOffsetMax = referenceBufferOffsetMin + dimention_of(referenceBuf);
				d = referenceBuf[0]; 
				return true;
			}
		}
		return false;
	}
};
void AutomaticOptionsTresholds::SubUpdate(wchar_t *letter, unsigned color)
{
	WIN32_FIND_DATA fd; 
	wsprintf(subPath, L"%s\\*.dat", letter); 
	HANDLE hFind = ::FindFirstFile(path, &fd); 
	if(hFind != INVALID_HANDLE_VALUE) 
	{ 
		do 
		{ 
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
			{
				wsprintf(subPath, L"%s\\%s", letter, fd.cFileName); 
				DataBufferX dataBuff(path);
				if(dataBuff.ok)
				{
					int start = int(0.1 * dataBuff.currentOffset);
					int stop = dataBuff.currentOffset - start;
					double inputs[1024] = {};
					compute.SubCompute(
						tresholds
						, start
						, stop
						, dataBuff
						, inputs
						);
					Data data;
					memcpy(data.tresholds, corel.inputItem.elements, sizeof(data.tresholds));
					data.Name = letter;
					data.color = color;
					data.File = fd.cFileName;
					thresholdsData.push_back(data);
				}
			}
		}while(::FindNextFile(hFind, &fd)); 
		::FindClose(hFind); 
	} 
}

void AutomaticOptionsTresholds::Update()
{
	thresholdsData.clear();
	for(Corel::TClassTubeItem::const_iterator i = corel.classTubeItem.cbegin(); i != corel.classTubeItem.cend(); ++i)
	{
		SubUpdate((wchar_t *)i->second->Name.c_str(), i->second->color);
	}
    SubUpdate(L"TEST", -1);
	corel.inputItem.classTube = 0;
	memset(corel.inputItem.elements, 0, sizeof(corel.inputItem.elements));
	for( std::vector<Corel::SolidTubeItem *>::iterator i = corel.solidTubeItem.begin(); i != corel.solidTubeItem.end(); ++i)
	{
		(*i)->correlation = 0;
	}
}

unsigned __color__[] = {
	  0xffff0000 
    , 0xff0000ff
	, 0xff00ff00
	, 0xffffc0cb
	, 0xff3a4345
	, 0xffcdd7f7
	, 0xfff2b50f
	, 0xff30eeb4
	, 0xffe900f9
	, 0xffcf641e
};

void AutomaticOptionsTresholds::Init()
{
	typedef AutomaticOptionsTresholdsNameSpace::SelectWithWapper<ThresholdsTable::items_list>::Result list;
	TL::foreach<list, AutomaticOptionsTresholdsNameSpace::__tresholds__>()(
		&Singleton<ThresholdsTable>::Instance().items
		, tresholds
		);
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		CurrentParametersTable &param = Singleton<CurrentParametersTable>::Instance();
		Select<CurrentParametersTable>(base).ID(1).Execute(param);
		int id = param.items.get<CurrentID>().value;
		ParametersTable typeSizeParam;
		Select<ParametersTable>(base).ID(id).Execute(typeSizeParam);	
		GetModuleFileName(0, path, 1024);
		PathRemoveFileSpec(path);
		wcscat(path, L"\\ConfigThresholds\\");
		CreateDirectory(path, NULL);
		wchar_t *s = &path[wcslen(path)];
		wchar_t *txt = typeSizeParam.items.get<NameParam>().value;
		wsprintf(s, L"%s\\", txt);
		CreateDirectory(path, NULL);
        s = &s[wcslen(s)];
		subPath = s;
		int k = 0;
		for(Corel::TClassTubeItem::const_iterator i = corel.classTubeItem.cbegin(); i != corel.classTubeItem.cend(); ++i)
		{
			wsprintf(s, L"%s\\", i->second->Name.c_str());
			CreateDirectory(path, NULL);	
			i->second->color = __color__[k];
			++k;
		}
		Update();
	}
}
//---------------------------------------------------------------------------


