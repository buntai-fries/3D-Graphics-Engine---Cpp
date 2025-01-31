#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void ShutDown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

private:
	bool InitializeBuffer(ID3D11Device*);
	void ShutDownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
#endif
