#pragma once

namespace Automat
{
	extern void(*dataChanged)(int);
	void Init();
	void Destroy();
	void Start();
	void Stop();
}