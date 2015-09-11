#pragma once

#if 0
class StoredData
{
	int num;
public:
	StoredData();
	void Store(wchar_t *, wchar_t *);
 
};

extern StoredData storedData;
#else
class StoredData
{
	int num;
	StoredData();
public:	
	void Store(wchar_t *diametre, wchar_t *gr);
	int Current();
	void Inc();
	void Clear();
	static StoredData &Instance();
};
#endif