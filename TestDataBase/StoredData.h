#pragma once
class StoredData
{
	int num;
	StoredData(void);
public:	
	void Store(wchar_t *diametre, wchar_t *gr);
	int Current();
	void Clear();
	static StoredData &Instance();
};


