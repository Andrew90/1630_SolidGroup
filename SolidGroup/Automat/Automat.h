#pragma once

namespace Automat
{
	extern void(*dataChanged)();
	void Init();
	void Destroy();
	void Start();
	void Stop();
}