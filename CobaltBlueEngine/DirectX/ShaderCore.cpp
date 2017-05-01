#include "ShaderCore.h"

ShaderCore::ShaderCore()
{
  m_3DvertexShader = 0;
  m_3DpixelShader = 0;
  m_2DvertexShader = 0;
  m_2DpixelShader = 0;
  m_layout = 0;
  m_2DmatrixBuffer = 0;
  m_3DmatrixBuffer = 0;
  m_2DsampleState = 0;
  m_3DsampleState = 0;
}

ShaderCore::~ShaderCore()
{
  Release();
}

void ShaderCore::Initialize(ID3D11Device* device, HWND content)
{
  InitializeShader2D(device, content, L"Shaders/Render2D.vs", L"Shaders/Render2D.ps", &m_2DpixelShader, &m_2DvertexShader);
  InitializeShader2D(device, content, L"Shaders/Render2D.vs", L"Shaders/Render2DL.ps", &m_2DpixelShaderLight, &m_2DvertexShader);
  InitializeShader3D(device, content, L"Shaders/Render3D.vs", L"Shaders/Render3D.ps");
}

void ShaderCore::Release()
{
  // Release the matrix constant buffer.
  if (m_2DmatrixBuffer)
  {
    m_2DmatrixBuffer->Release();
    m_2DmatrixBuffer = 0;
  }

  // Release the matrix constant buffer.
  if (m_3DmatrixBuffer)
  {
    m_3DmatrixBuffer->Release();
    m_3DmatrixBuffer = 0;
  }

  // Release the layout.
  if (m_layout)
  {
    m_layout->Release();
    m_layout = 0;
  }

  // Release the pixel shader.
  if (m_2DpixelShader)
  {
    m_2DpixelShader->Release();
    m_2DpixelShader = 0;
  }

  // Release the vertex shader.
  if (m_2DvertexShader)
  {
    m_2DvertexShader->Release();
    m_2DvertexShader = 0;
  }

  // Release the pixel shader.
  if (m_3DpixelShader)
  {
    m_3DpixelShader->Release();
    m_3DpixelShader = 0;
  }

  // Release the vertex shader.
  if (m_3DvertexShader)
  {
    m_3DvertexShader->Release();
    m_3DvertexShader = 0;
  }

  // Release the sampler state.
  if (m_2DsampleState)
  {
    m_2DsampleState->Release();
    m_2DsampleState = 0;
  }

  if (m_3DsampleState)
  {
    m_3DsampleState->Release();
    m_3DsampleState = 0;
  }
}

bool ShaderCore::Render2D(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, D3DXVECTOR4 translate, D3DXVECTOR4 color, D3DXVECTOR2 dimensions, ID3D11ShaderResourceView* texture)
{
  // Set the shader parameters that it will use for rendering.
  SetShaderParameters2D(context, worldMatrix, viewMatrix, projectionMatrix, translate, color, dimensions, texture);

  // Now render the prepared buffers with the shader.
  RenderShader2D(context, indexCount, m_2DpixelShader, m_2DvertexShader);

  return true;
}

bool ShaderCore::RenderLight2D(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, D3DXVECTOR4 translate, D3DXVECTOR4 color, D3DXVECTOR2 dimensions, ID3D11ShaderResourceView* texture)
{
  // Set the shader parameters that it will use for rendering.
  SetShaderParameters2D(context, worldMatrix, viewMatrix, projectionMatrix, translate, color, dimensions, texture);

  // Now render the prepared buffers with the shader.
  RenderShader2D(context, indexCount, m_2DpixelShaderLight, m_2DvertexShader);

  return true;
}

bool ShaderCore::Render3D(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters3D(context, worldMatrix, viewMatrix, projectionMatrix, texture);

	// Now render the prepared buffers with the shader.
	RenderShader3D(context, indexCount);

	return true;
}

void ShaderCore::InitializeShader2D(ID3D11Device* device, HWND content, WCHAR* vertexFile, WCHAR* pixelFile, ID3D11PixelShader** pixelShader, ID3D11VertexShader** vectorShader)
{
  HRESULT result;
  ID3D10Blob* errorMessage;
  ID3D10Blob* vertexShaderBuffer;
  ID3D10Blob* pixelShaderBuffer;
  D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
  unsigned int numElements;
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;

  // Initialize the pointers this function will use to null.
  errorMessage = 0;
  vertexShaderBuffer = 0;
  pixelShaderBuffer = 0;

  // Compile the vertex shader code.
  result = D3DX11CompileFromFile(vertexFile, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &vertexShaderBuffer, &errorMessage, 0);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, content, vertexFile);
    }
    // If there was  nothing in the error message then it simply could not find the shader file itself.
    else
    {
      MessageBox(content, vertexFile, L"Missing Shader File", MB_OK);
      ExitProcess(-1);
    }

    return;
  }

  // Compile the pixel shader code.
  result = D3DX11CompileFromFile(pixelFile, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &pixelShaderBuffer, &errorMessage, 0);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, content, pixelFile);
    }
    // If there was nothing in the error message then it simply could not find the file itself.
    else
    {
      MessageBox(content, pixelFile, L"Missing Shader File", MB_OK);
      ExitProcess(-1);
    }

    return;
  }

  // Create the vertex shader from the buffer.
  device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, vectorShader);

  // Create the pixel shader from the buffer.
  device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, pixelShader);

  // Create the vertex input layout description.
  // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  polygonLayout[1].SemanticName = "COLOR";
  polygonLayout[1].SemanticIndex = 0;
  polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  polygonLayout[1].InputSlot = 0;
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "TEXCOORD";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  // Get a count of the elements in the layout.
  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  // Create the vertex input layout.
  device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);

  // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
  vertexShaderBuffer->Release();
  vertexShaderBuffer = 0;

  pixelShaderBuffer->Release();
  pixelShaderBuffer = 0;

  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(BufferType2D);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  device->CreateBuffer(&matrixBufferDesc, NULL, &m_2DmatrixBuffer);

  // Create a texture sampler state description.
  samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 4;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // Create the texture sampler state.
  device->CreateSamplerState(&samplerDesc, &m_2DsampleState);
}

void ShaderCore::InitializeShader3D(ID3D11Device* device, HWND content, WCHAR* vertexShader, WCHAR* pixelShader)
{
  HRESULT result;
  ID3D10Blob* errorMessage;
  ID3D10Blob* vertexShaderBuffer;
  ID3D10Blob* pixelShaderBuffer;
  D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
  unsigned int numElements;
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;

  // Initialize the pointers this function will use to null.
  errorMessage = 0;
  vertexShaderBuffer = 0;
  pixelShaderBuffer = 0;

  // Compile the vertex shader code.
  result = D3DX11CompileFromFile(vertexShader, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &vertexShaderBuffer, &errorMessage, 0);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, content, vertexShader);
    }
    // If there was  nothing in the error message then it simply could not find the shader file itself.
    else
    {
      MessageBox(content, vertexShader, L"Missing Shader File", MB_OK);
    }

    return;
  }

  // Compile the pixel shader code.
  result = D3DX11CompileFromFile(pixelShader, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &pixelShaderBuffer, &errorMessage, 0);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, content, pixelShader);
    }
    // If there was nothing in the error message then it simply could not find the file itself.
    else
    {
      MessageBox(content, pixelShader, L"Missing Shader File", MB_OK);
    }

    return;
  }

  // Create the vertex shader from the buffer.
  device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_3DvertexShader);

  // Create the pixel shader from the buffer.
  device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_3DpixelShader);

  // Create the vertex input layout description.
  // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  polygonLayout[1].SemanticName = "COLOR";
  polygonLayout[1].SemanticIndex = 0;
  polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  polygonLayout[1].InputSlot = 0;
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "TEXCOORD";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  // Get a count of the elements in the layout.
  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  // Create the vertex input layout.
  device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);

  // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
  vertexShaderBuffer->Release();
  vertexShaderBuffer = 0;

  pixelShaderBuffer->Release();
  pixelShaderBuffer = 0;

  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(BufferType3D);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  device->CreateBuffer(&matrixBufferDesc, NULL, &m_3DmatrixBuffer);

  // Create a texture sampler state description.
  samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 4;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // Create the texture sampler state.
  device->CreateSamplerState(&samplerDesc, &m_3DsampleState);
}

void ShaderCore::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
  char* compileErrors;
  unsigned long long bufferSize, i;
  ofstream fout;


  // Get a pointer to the error message text buffer.
  compileErrors = (char*)(errorMessage->GetBufferPointer());

  // Get the length of the message.
  bufferSize = errorMessage->GetBufferSize();

  // Open a file to write the error message to.
  fout.open("shader-error.txt");

  // Write out the error message.
  for (i = 0; i<bufferSize; i++)
  {
    fout << compileErrors[i];
  }

  // Close the file.
  fout.close();

  // Release the error message.
  errorMessage->Release();
  errorMessage = 0;

  // Pop a message up on the screen to notify the user to check the text file for compile errors.
  MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

  ExitProcess(-1);
}

void ShaderCore::SetShaderParameters2D(ID3D11DeviceContext* context, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, D3DXVECTOR4 translate, D3DXVECTOR4 color, D3DXVECTOR2 dimensions, ID3D11ShaderResourceView* texture)
{
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  BufferType2D* dataPtr;
  unsigned int bufferNumber;

  // Transpose the matrices to prepare them for the shader.
  D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
  D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
  D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

  // Lock the constant buffer so it can be written to.
  context->Map(m_2DmatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  dataPtr = (BufferType2D*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;
  dataPtr->trans = translate;
  dataPtr->color = color;
  dataPtr->metrics = dimensions;

  // Unlock the constant buffer.
  context->Unmap(m_2DmatrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Finanly set the constant buffer in the vertex shader with the updated values.
  context->VSSetConstantBuffers(bufferNumber, 1, &m_2DmatrixBuffer);

  // Set shader texture resource in the pixel shader.
  context->PSSetShaderResources(0, 1, &texture);
}

void ShaderCore::SetShaderParameters3D(ID3D11DeviceContext* context, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture)
{
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  BufferType3D* dataPtr;
  unsigned int bufferNumber;

  // Transpose the matrices to prepare them for the shader.
  D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
  D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
  D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

  // Lock the constant buffer so it can be written to.
  context->Map(m_3DmatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  dataPtr = (BufferType3D*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  // Unlock the constant buffer.
  context->Unmap(m_3DmatrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Finanly set the constant buffer in the vertex shader with the updated values.
  context->VSSetConstantBuffers(bufferNumber, 1, &m_3DmatrixBuffer);

  // Set shader texture resource in the pixel shader.
  context->PSSetShaderResources(0, 1, &texture);
}

void ShaderCore::RenderShader2D(ID3D11DeviceContext* context, int indexCount, ID3D11PixelShader* pixelShader, ID3D11VertexShader* vectorShader)
{
  // Set the vertex input layout.
  context->IASetInputLayout(m_layout);

  // Set the vertex and pixel shaders that will be used to render this triangle.
  context->VSSetShader(vectorShader, NULL, 0);
  context->PSSetShader(pixelShader, NULL, 0);

  // Set the sampler state in the pixel shader.
  context->PSSetSamplers(0, 1, &m_2DsampleState);

  // Render the triangle.
  context->DrawIndexed(indexCount, 0, 0);
}

void ShaderCore::RenderShader3D(ID3D11DeviceContext* context, int indexCount)
{
	// Set the vertex input layout.
	context->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	context->VSSetShader(m_3DvertexShader, NULL, 0);
	context->PSSetShader(m_3DpixelShader, NULL, 0);

  // Set the sampler state in the pixel shader.
  context->PSSetSamplers(0, 1, &m_3DsampleState);

	// Render the triangle.
	context->DrawIndexed(indexCount, 0, 0);
}
