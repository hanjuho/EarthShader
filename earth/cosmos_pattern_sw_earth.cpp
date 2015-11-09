#include "hcosmos.h"



using namespace h;



Embedded_Cosmos::Earth_SW::Earth_SW(DWORD id)
:Cosmos_SW(id),
my_specTex(NULL),
my_bumpTex(NULL),
my_baseTexHandle(NULL),
my_specTexHandle(NULL),
my_bumpTexHandle(NULL),
my_attractionHandle(NULL),
my_gravityHandle(NULL),
my_attraction(0.0f, 0.0f, 0.0f, 0.0f)
{
}



Embedded_Cosmos::Earth_SW::~Earth_SW(VOID)
{
	DEL_COM(my_specTex);
	DEL_COM(my_bumpTex);
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Earth_SW, initialize)(ptr_Cosmos hw, ptr_Cosmos_Planet planet)
{
	HRESULT hr = Cosmos_SW::initialize(hw, planet);
	IF_FAILED(hr)
	{
		return hr;
	}

	hdx_3DDevice device = planet->get_Device();
	hr = ADD_FLAG(hr, my_pixel.initialize(device, TEXT("..\\resource/ocean_shader.hlsl")));

	IF_FAILED(hr)
	{
		return hr;
	}

	hdx_load_Texture(device, TEXT("..\\resource/earthspec.jpg"), &my_specTex);
	hdx_load_Texture(device, TEXT("..\\resource/earthbump.jpg"), &my_bumpTex);

	IF_FAILED(hr)
	{
		return hr;
	}

	my_attractionHandle = my_pixel.table->GetConstantByName(0, "Attraction");
	my_gravityHandle = my_pixel.table->GetConstantByName(0, "Gravity");
	my_baseTexHandle = my_pixel.table->GetConstantByName(0, "BaseTex");
	my_specTexHandle = my_pixel.table->GetConstantByName(0, "SpecTex");
	my_bumpTexHandle = my_pixel.table->GetConstantByName(0, "BumpTex");

	UINT count;
	my_pixel.table->GetConstantDesc(my_baseTexHandle, &my_baseTexDesc, &count);
	my_pixel.table->GetConstantDesc(my_specTexHandle, &my_specTexDesc, &count);
	my_pixel.table->GetConstantDesc(my_bumpTexHandle, &my_bumpTexDesc, &count);

	return hr;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Earth_SW, VOID, update)(FLOAT delta)
{
	hdx_3DDevice device = my_planet->get_Device();

	my_attraction.x = FLOAT(my_cosmos->acc_velocity[0]) * 1000.0f;
	my_attraction.y = FLOAT(my_cosmos->acc_velocity[1]) * 1000.0f;
	my_attraction.z = FLOAT(my_cosmos->acc_velocity[2]) * 1000.0f;

	my_pixel.table->SetFloat(device, my_gravityHandle, FLOAT(G_EARTH));
	my_pixel.table->SetVector(device, my_attractionHandle, &my_attraction);

	Cosmos_SW::update(delta);
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Earth_SW, render)(VOID)
{
	hdx_3DDevice device = my_planet->get_Device();

	IF_SUCCEEDED(my_pixel.begin())
	{
		// base tex
		device->SetSamplerState(my_baseTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(my_baseTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(my_baseTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// spec tex
		device->SetTexture(my_specTexDesc.RegisterIndex, my_specTex);

		// bump tex
		device->SetTexture(my_bumpTexDesc.RegisterIndex, my_bumpTex);

		my_planet->render_mesh();
		my_pixel.end();
	}

	my_planet->render_particle();

	return S_OK;
}