#include "Application.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;
	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not intialized the Direct3D", L"Error", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}


void ApplicationClass::Shutdown()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool ApplicationClass::Frame()
{
	bool result;
	//Render the graphics scene
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	// Clear the buffer to begin the scene
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	// Present the render scene to the screen
	m_Direct3D->EndScene();

	return true;
}
