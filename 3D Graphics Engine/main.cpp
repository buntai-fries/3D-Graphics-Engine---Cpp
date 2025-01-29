#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	System = new SystemClass;
	bool result = System->Initialize();

	if (result)
	{
		System->Run();
	}

	System->ShutDown();
	delete System;
	System = 0;
	return 0;
}