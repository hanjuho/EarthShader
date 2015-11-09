#pragma once



#include "hdxdefbase.h"
#include "houtputbase.h"



namespace h
{
	DLL_DECL_STRUCT(hdx_Material_Info)
	{
		typedef D3DMATERIAL9 MATERIAL;
		typedef IDirect3DTexture9 * TEXTURE;
		hdx_3DDevice	device;
		DWORD			level;
		MATERIAL		material;
		TEXTURE			texture;
	} *ptr_DxMaterial_Info;

	DLL_BASE_INHERITANCE(hdx_Material, h_Renderer)
	{
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST DWORD level = 0)PURE;
		INTERFACE_DECL_FUNC(load_Texture)			(LPCWSTR filename)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Level)		(CONST DWORD level);
		INTERFACE_DECL_FUNC_T(VOID, set_Material)	(CONST D3DMATERIAL9 * pMaterial)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Texture)	(CONST hdx_Material_Info::TEXTURE texture)PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_DxMaterial_Info info)CONST PURE;
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)(VOID)PURE;
	} *ptr_DxMaterial;

	DLL_DECL_INHERITANCE(CLASS, hdx_Material_Own, hdx_Material)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_Material_Own)		(CONST D3DMATERIAL9 * pMaterial = NULL);
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST DWORD level = 0);
		INTERFACE_DECL_FUNC(load_Texture)			(LPCWSTR filename);
		INTERFACE_DECL_FUNC_T(VOID, set_Level)		(CONST DWORD level);
		INTERFACE_DECL_FUNC_T(VOID, set_Material)	(CONST D3DMATERIAL9 * pMaterial);
		INTERFACE_DECL_FUNC_T(VOID, set_Texture)	(CONST hdx_Material_Info::TEXTURE texture);
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_DxMaterial_Info info)CONST;
		INTERFACE_DECL_FUNC(render_before)			(VOID);
		INTERFACE_DECL_FUNC(render_after)			(VOID);
		INTERFACE_DECL_FUNC(render)					(VOID);
		INTERFACE_DECL_FUNC(reset)					(VOID);
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)(VOID);
	private:
		hdx_3DDevice				my_device;
		DWORD						my_level;
		hdx_Material_Info::MATERIAL	my_material;
		hdx_Material_Info::TEXTURE	my_texture;
	} *ptr_DxMaterial_Own;
};