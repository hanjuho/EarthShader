#include "hdxdefbase.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_PixelShader)(VOID)
: device(NULL), table(NULL), shader(NULL)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_PixelShader)(VOID)
{
	DEL_COM(device);
	DEL_COM(table);
	DEL_COM(shader);
}



CLASS_REALIZE_FUNC(hdx_PixelShader, initialize)(hdx_3DDevice device, LPCWSTR script, LPCSTR entry, LPCSTR version)
{
	DEL_COM(this->device);
	DEL_COM(table);
	DEL_COM(shader)

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
		// output any error messages
		IF_NOT_NULL(errorBuffer)
		{
			MessageBoxA(0, (LPCSTR)errorBuffer->GetBufferPointer(), 0, 0);
			DEL_COM(errorBuffer);
		}

		return hr;
	}

	IF_FAILED(hr = device->CreatePixelShader(
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



CLASS_REALIZE_FUNC(hdx_PixelShader, begin)(VOID)
{
	return device->SetPixelShader(shader);
}



CLASS_REALIZE_FUNC(hdx_PixelShader, end)(VOID)
{
	return device->SetPixelShader(NULL);
}