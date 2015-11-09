#include "hdxmesh.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_Mesh_Own)(VOID)
: my_device(NULL), 
my_materials(0), 
my_mesh(NULL)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_Mesh_Own)(VOID)
{
	reset();
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, initialize)(hdx_3DDevice device, LPCWSTR filename)
{
	IF_NULL(device)
	{
		return E_INVALIDARG;
	}

	IF_NOT_NULL(my_device)
	{
		return E_ACCESSDENIED;
	}

	my_device = device;
	my_device->AddRef();
	load_Mesh(filename);

	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, load_Mesh)(LPCWSTR filename)
{
	hdx_Mesh_Info::MESH mesh;
	LPD3DXBUFFER adjBuffer;
	LPD3DXBUFFER mtrlBuffer;
	DWORD        numMtrls;

	HRESULT hr;
	IF_FAILED(hr = D3DXLoadMeshFromX(
		filename,
		D3DXMESH_MANAGED,
		my_device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&mesh))
	{
		return hr;
	}

	IF_NOT_NULL((mtrlBuffer && adjBuffer))
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		set_NumOfMaterial(numMtrls);
		for (DWORD index = numMtrls - 1; index != -1; --index)
		{
			mtrls[index].MatD3D.Ambient = mtrls[index].MatD3D.Diffuse;
			set_Material(index, &mtrls[index].MatD3D);
		
			// multibyte -> unicode º¯È¯
			TCHAR unicode[256];
			CHAR multibyte[256];
			strcpy_s(multibyte, 256, mtrls[index].pTextureFilename);
			int nLen = MultiByteToWideChar(CP_ACP, 0, multibyte, strlen(multibyte), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, multibyte, strlen(multibyte), unicode, nLen);

			hdx_Material_Info::TEXTURE texture;
			IF_SUCCEEDED(hdx_load_Texture(my_device, unicode, &texture))
			{
				set_Texture(0, texture);
			}			
		}
	}
	DEL_COM(mtrlBuffer);

	mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);
	DEL_COM(adjBuffer);

	set_Mesh(mesh);

	return hr;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, load_Texture)(DWORD index, LPCWSTR filename)
{
	return my_materials[index]->load_Texture(filename);
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, VOID, set_NumOfMaterial)(DWORD size)
{
	hdx_Mesh_Info::MATERIALS materials(size);
	for (DWORD index = materials.size() - 1; index != -1; --index)
	{
		try
		{
			materials[index] = new hdx_Material_Own();
			materials[index]->initialize(my_device, index);
		}
		catch (...)
		{
			return;
		}
	}
	
	for (DWORD index = my_materials.size() - 1; index != -1; --index)
	{
		DEL_POINTER(my_materials[index]);
	}

	my_materials = materials;
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, VOID, set_Material)(DWORD level, CONST D3DMATERIAL9 * pMaterial)
{
	my_materials[level]->set_Material(pMaterial);
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, VOID, set_Texture)(DWORD level, CONST hdx_Material_Info::TEXTURE texture)
{
	my_materials[level]->set_Texture(texture);
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, VOID, set_Mesh)(CONST hdx_Mesh_Info::MESH mesh)
{
	DEL_COM(my_mesh);
	my_mesh = mesh;
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, DWORD, get_NumOfMaterial)(VOID)
{
	return my_materials.size();
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, VOID, get_info)(ptr_DxMesh_Info info)CONST
{
	info->device = my_device;
	info->materials = my_materials;
	info->mesh = my_mesh;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, render_before)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	hr = ADD_FLAG(hr, my_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	hr = ADD_FLAG(hr, my_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT));
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, render_after)(VOID)
{
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, render)(VOID)
{
	IF_TRUE(my_materials.empty())
	{
		return my_mesh->DrawSubset(0);
	}

	for (DWORD index = my_materials.size() - 1; index != -1; --index)
	{
		IF_SUCCEEDED(my_materials[index]->render_before())
		{
			my_materials[index]->render();
			my_mesh->DrawSubset(index);
			my_materials[index]->render_after();
		}
	}

	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_Mesh_Own, reset)(VOID)
{
	DEL_COM(my_device);
	set_NumOfMaterial(0);
	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_Mesh_Own, hdx_3DDevice, get_Device)(VOID)
{
	return my_device;
}