#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// User defined header file
#include "InputClass.h"
#include "Application.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void ShutDown();
	void Run();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutDownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// Function Prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals
static SystemClass* ApplicationHandle = 0;

#endif