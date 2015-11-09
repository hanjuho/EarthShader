#include "hdxmaterial.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_Material_Own)(CONST D3DMATERIAL9 * pMaterial)
: my_device(NULL), 
my_level(0), 
my_texture(0), 
my_material(hdx_BLACK_MTRL)
{
	IF_NOT_NULL(pMaterial)
	{
		set_Material(pMaterial);
	}
}



CLASS_REALIZE_DESTRUCTOR(hdx_Material_Own)(VOID)
{
	reset();
}



CLASS_REALIZE_FUNC(hdx_Material_Own, initialize)(CONST hdx_3DDevice device, DWORD level)
{
	IF_NULL(device)
	{
		return E_INVALIDARG;
	}

	IF_NOT_NULL(my_device)
	{
		return E_HANDLE;
	}

	my_device = device;
	my_device->AddRef();
	set_Level(level);

	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Material_Own, load_Texture)(LPCWSTR filename)
{
	hdx_Material_Info::TEXTURE texture;
	HRESULT hr;
	IF_FAILED(hr = hdx_load_Texture(my_device, filename, &texture))
	{
		return hr;
	}

	set_Texture(texture);

	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_Material_Own, VOID, set_Level)(CONST DWORD level)
{
	my_level = level;
}



CLASS_REALIZE_FUNC_T(hdx_Material_Own, VOID, set_Material)(CONST D3DMATERIAL9 * pMaterial)
{
	my_material = *pMaterial;
}



CLASS_REALIZE_FUNC_T(hdx_Material_Own, VOID, set_Texture)(CONST hdx_Material_Info::TEXTURE texture)
{
	DEL_COM(my_texture);
	my_texture = texture;
}



CLASS_REALIZE_FUNC_T(hdx_Material_Own, VOID, get_info)(ptr_DxMaterial_Info info)CONST
{
	info->device = my_device;
	info->level = my_level;
	info->material = my_material;
	info->texture = my_texture;
}



CLASS_REALIZE_FUNC(hdx_Material_Own, render_before)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetMaterial(&my_material));
	hr = ADD_FLAG(hr, my_device->SetTexture(my_level, my_texture));
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Material_Own, render_after)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetMaterial(&hdx_BLACK_MTRL));
	hr = ADD_FLAG(hr, my_device->SetTexture(my_level, NULL));
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Material_Own, render)(VOID)
{
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Material_Own, reset)(VOID)
{
	DEL_COM(my_device);
	set_Level(0);
	set_Material(&hdx_BLACK_MTRL);
	set_Texture(NULL);
	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_Material_Own, hdx_3DDevice, get_Device)(VOID)
{
	return my_device;
}