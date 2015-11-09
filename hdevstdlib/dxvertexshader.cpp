#include "hdxdefbase.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_VertexShader)(VOID)
: device(NULL), table(NULL), shader(NULL)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_VertexShader)(VOID)
{
	DEL_COM(device);
	DEL_COM(table);
	DEL_COM(shader);
}



CLASS_REALIZE_FUNC(hdx_VertexShader, initialize)(hdx_3DDevice device, LPCWSTR script, LPCSTR entry, LPCSTR version)
{
	DEL_COM(this->device);
	DEL_COM(table);
	DEL_COM(shader);

	ID3DXBuffer* buffer = 0;
	ID3DXBuffer* errorBuffer = 0;

	// vertex begin
	HRESULT hr;
	IF_FAILED(hr = D3DXCompileShaderFromFile(
		script,
		0,
		0,
		entry,
		version,
		D3DXSHADER_DEBUG,
		&buffer,
		&errorBuffer,
		&table))
	{
		return hr;
	}

	// output any error messages
	IF_NOT_NULL(errorBuffer)
	{
		MessageBoxA(0, (LPCSTR)errorBuffer->GetBufferPointer(), 0, 0);
		DEL_COM(errorBuffer);
	}

	IF_FAILED(hr = device->CreateVertexShader(
		(LPDWORD)buffer->GetBufferPointer(),
		&shader))
	{
		DEL_COM(table);
		return hr;
	}

	device->AddRef();
	this->device = device;

	return hr;
}



CLASS_REALIZE_FUNC(hdx_VertexShader, begin)(VOID)
{
	return device->SetVertexShader(shader);
}



CLASS_REALIZE_FUNC(hdx_VertexShader, end)(VOID)
{
	return device->SetVertexShader(NULL);
}