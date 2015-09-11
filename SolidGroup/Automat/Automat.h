#pragma once

class Automat
{
	bool contine;
public:
	bool start;
private:
	static void Do(Automat *);
public:
	static void (*dataChanged)();
	Automat();
	void Init();
	void Destroy();
	void Start();
	void Stop();
};

extern Automat automat;

void ComputeStep(unsigned );