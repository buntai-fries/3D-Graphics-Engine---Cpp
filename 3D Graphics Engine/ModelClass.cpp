#include "ModelClass.h"

ModelClass::ModelClass()
{

}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device*)
{
	return false;
}

void ModelClass::ShutDown()
{
}

void ModelClass::Render(ID3D11DeviceContext*)
{
}

int ModelClass::GetIndexCount()
{
	return 0;
}

bool ModelClass::InitializeBuffer(ID3D11Device*)
{
	return false;
}

void ModelClass::ShutDownBuffers()
{
}

void ModelClass::RenderBuffers(ID3D11DeviceContext*)
{
}
