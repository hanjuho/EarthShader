#pragma once



#include "hdxdefbase.h"
#include "hdxmaterial.h"
#include "houtputbase.h"



namespace h
{
	DLL_DECL_STRUCT(hdx_Mesh_Info)
	{
		typedef ID3DXMesh *	MESH;
		typedef std::vector<ptr_DxMaterial> MATERIALS;
		hdx_3DDevice	device;
		MATERIALS		materials;
		MESH			mesh;
	} *ptr_DxMesh_Info;

	DLL_BASE_INHERITANCE(hdx_Mesh, h_Renderer)
	{
		INTERFACE_DECL_FUNC(initialize)					(hdx_3DDevice device, LPCWSTR filename = NULL)PURE;
		INTERFACE_DECL_FUNC(load_Mesh)					(LPCWSTR filename)PURE;
		INTERFACE_DECL_FUNC(load_Texture)				(DWORD index, LPCWSTR filename)PURE;
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfMaterial)	(VOID)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_NumOfMaterial)	(DWORD size)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Material)		(DWORD level, CONST D3DMATERIAL9 * pMaterial)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Texture)		(DWORD level, CONST hdx_Material_Info::TEXTURE texture)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Mesh)			(CONST hdx_Mesh_Info::MESH mesh)PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_info)			(ptr_DxMesh_Info info)CONST PURE;
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)	(VOID)PURE;
	} *ptr_DxMesh;

	DLL_DECL_INHERITANCE(CLASS, hdx_Mesh_Own, hdx_Mesh)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_Mesh_Own)				(VOID);
		INTERFACE_DECL_FUNC(initialize)					(hdx_3DDevice device, LPCWSTR filename = NULL);
		INTERFACE_DECL_FUNC(load_Mesh)					(LPCWSTR filename);
		INTERFACE_DECL_FUNC(load_Texture)				(DWORD index, LPCWSTR filename);
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfMaterial)	(VOID);
		INTERFACE_DECL_FUNC_T(VOID, set_NumOfMaterial)	(DWORD size);
		INTERFACE_DECL_FUNC_T(VOID, set_Material)		(DWORD level, CONST D3DMATERIAL9 * pMaterial);
		INTERFACE_DECL_FUNC_T(VOID, set_Texture)		(DWORD level, CONST hdx_Material_Info::TEXTURE texture);
		INTERFACE_DECL_FUNC_T(VOID, set_Mesh)			(CONST hdx_Mesh_Info::MESH mesh);
		INTERFACE_DECL_FUNC_T(VOID, get_info)			(ptr_DxMesh_Info info)CONST;
		INTERFACE_DECL_FUNC(render_before)				(VOID);
		INTERFACE_DECL_FUNC(render_after)				(VOID);
		INTERFACE_DECL_FUNC(render)						(VOID);
		INTERFACE_DECL_FUNC(reset)						(VOID);
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)	(VOID);
	private:
		hdx_3DDevice				my_device;
		hdx_Mesh_Info::MATERIALS	my_materials;
		hdx_Mesh_Info::MESH			my_mesh;
	} *ptr_DxMesh_Own;
};