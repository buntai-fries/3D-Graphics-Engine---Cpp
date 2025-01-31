#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
    m_layout = 0;
    m_matrixBuffer = 0;
    m_pixelShader = 0;
    m_vertexShader = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hWnd)
{
    bool result;
    wchar_t vs_file[128];
    wchar_t ps_file[128];
    int error;

    // Set the file-name of vertex shader
    error = wcscpy_s(vs_file, 128, L"D:/3D Engine/3D Graphics Engine/3D Graphics Engine/ShaderFiles/color.vs");
    if (error != 0)
    {
        return false;
    }

    // Set the file name of pixel shader
    error = wcscpy_s(ps_file, 128, L"D:/3D Engine/3D Graphics Engine/3D Graphics Engine/ShaderFiles/Color.ps");
    if (error != 0)
    {
        return false;
    }

    // Initialize the vertex and pixel shader
    result = InitializeShader(device, hWnd, vs_file, ps_file);
    if (!result)
    {
        return false;
    }
    return true;
}

void ColorShaderClass::ShutDown()
{
    ShutDownShader();
    return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, 
                              int indexCount, 
                              XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
    bool result;

    // Set the shader parameter that it will use for rendering
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
    {
        return false;
    }
    // Render prepared buffer with shader!
    RenderShader(deviceContext, indexCount);
    return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* device, 
                                        HWND hWnd, WCHAR* vs_file, WCHAR* ps_file)

{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;

    // Initialize the pointers this function will use to null.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;

    // Compiled the vertex shader code file
    result = D3DCompileFromFile(vs_file, NULL, NULL, 
            "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
            &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, vs_file);
        }
        else
        {
            MessageBox(hWnd, vs_file, L"Missing vsShader File", MB_ICONEXCLAMATION | MB_OK);
        }
        return false;
    }

    // Compile the pixel shader code file
    result = D3DCompileFromFile(ps_file, NULL, NULL,
            "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
            &pixelShaderBuffer, &errorMessage);

    if (FAILED(result))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, ps_file);
        }
        else
        {
            MessageBox(hWnd, ps_file, L"Missing psShader File", MB_OK|MB_ICONEXCLAMATION);
        }
        return false;
    }

    // Create the vertex shader from buffer
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // Create the pixel shader from buffer
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // Vertex Layout Description
    // This setup need to match the vertex-type structure in the model class and in the shader
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].SemanticName = "POSITION";

    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].SemanticName = "COLOR";

    // Get the number of element in layout
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create the vertex input layout
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
            vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Releasing the vertex and pixel shader
    vertexShaderBuffer->Release();
    vertexShaderBuffer = NULL;
    pixelShaderBuffer->Release();
    pixelShaderBuffer = NULL;

    // Set up description for dynamic matrix constant buffer in the vertex shader
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer ptr so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void ColorShaderClass::ShutDownShader()
{
    // Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = 0;
    }

    // Release the layout.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = 0;
    }

    // Release the pixel shader.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = 0;
    }

    // Release the vertex shader.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = 0;
    }

    return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFileName)
{
    char* compilerErrors;
    unsigned long long bufferSize, i;
    ofstream fout;

    // Get a ptr to the error msg text buffer
    compilerErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of messages
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader_error.txt");

    // Write the error message
    for (i = 0; i < bufferSize; i++)
    {
        fout << compilerErrors[i];
    }

    // Close the file
    fout.close();

    // Release the error
    errorMessage->Release();
    errorMessage = 0;

    // Pop the message to user-screen to check .txt file for compile errors
    MessageBox(hWnd, L"Error Compiling Shader. Check shader_error.txt for error msg.", 
               shaderFileName, MB_OK|MB_ICONEXCLAMATION);
    return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* devicecontext, 
                       XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;

    // Transpose the matrix to prepare for the shader
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    // Lock the buffer so that it can be written
    result = devicecontext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // Get a pointer to the data in the constant buffer.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // Copy the matrices into the constant buffer
    dataPtr->worldMatrix = worldMatrix;
    dataPtr->viewMatrix = worldMatrix;
    dataPtr->projectionMatrix = projectionMatrix;

    // Unlock the constant buffers
    devicecontext->Unmap(m_matrixBuffer, 0);

    // Set the position of the constant buffer in the vertex shader
    bufferNumber = 0;

    // Set the constant buffer in the vertex shader with the updated values
    devicecontext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // Set the virtual input layout
    deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shader that will be used to render this triangle
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // Render the triangle
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
