#include "hcosmos.h"



using namespace h;



Embedded_Cosmos::Cosmos_Planet::Cosmos_Planet(VOID)
: my_device(NULL)
{
	D3DXMatrixIdentity(&my_form);
}



Embedded_Cosmos::Cosmos_Planet::~Cosmos_Planet(VOID)
{
	my_particle.reset();
	my_mesh.reset();
	DEL_COM(my_device);
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Planet, initialize)(hdx_3DDevice device, LPCWSTR filename)
{
	IF_NULL(device)
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	IF_FAILED(hr = co_init_Planet(device, filename, &my_mesh))
	{
		return hr;
	}

	IF_FAILED(hr = my_particle.initialize(device, 512))
	{
		IF_TRUE(IS_FLAG(hr, E_HANDLE))
		{
			IF_SUCCEEDED(my_particle.reset())
			{
				hr = my_particle.initialize(device);
			}
		}

		IF_FAILED(hr)
		{
			return hr;
		}
	}

	DEL_COM(my_device);
	my_device = device;
	my_device->AddRef();

	return hr;
}




CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, update)(CONST FLOAT delta_time, ptr_DxParticle_Controller controller)
{
	my_particle.update(delta_time, controller);
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Planet, render_mesh)(VOID)
{
	my_device->SetTransform(D3DTS_WORLD, &my_form);

	IF_SUCCEEDED(my_mesh.render_before())
	{
		my_mesh.render();
		my_mesh.render_after();
	}

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Planet, render_particle)(VOID)
{
	my_device->SetTransform(D3DTS_WORLD, &hdx_mNor);

	IF_SUCCEEDED(my_particle.render_before())
	{
		my_particle.render();
		my_particle.render_after();
	}

	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, set_PerCreate)(CONST DWORD num)
{
	my_particle.set_PerCreate(num);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, set_AtomMax)(CONST DWORD atm_max)
{
	my_particle.set_AtomMax(atm_max);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, set_AtomLife)(CONST FLOAT time)
{
	my_particle.set_AtomLife(time);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, set_AtomRadius)(CONST FLOAT min, CONST FLOAT max)
{
	my_particle.set_AtomRadius(1.5f, min, max);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, VOID, set_CreateCycle)(CONST FLOAT time)
{
	my_particle.set_CreateCycle(time);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Planet, hdx_3DDevice, get_Device)(VOID)
{
	return my_device;
}