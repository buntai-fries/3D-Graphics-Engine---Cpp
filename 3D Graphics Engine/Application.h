#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void VideoCardDesc();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
};

#endif