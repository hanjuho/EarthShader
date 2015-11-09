#include "hdxparticle.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_Particle_Own)(VOID)
: my_device(NULL),
my_buffer(NULL),
my_atms(0),
my_per_create(1),
my_atm_max(0),
my_atm_life(0.0f),
my_atm_radius_scale(1.0f),
my_atm_radius_min(0.0f),
my_atm_radius_max(1.0f),
my_create_time(0.0f),
my_create_cycle(0.0f)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_Particle_Own)(VOID)
{
	reset();
}



CLASS_REALIZE_FUNC(hdx_Particle_Own, initialize)(CONST hdx_3DDevice device, CONST DWORD atm_max)
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

	set_AtomMax(atm_max);

	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, update)(CONST FLOAT delta_time, ptr_DxParticle_Controller controller)
{
	hdx_Particle_Info::ATOMS::iterator begin = my_atms.begin();
	hdx_Particle_Info::ATOMS::iterator end = my_atms.end();
	DWORD count = 0;
	while (begin != end)
	{
		hdx_Particle_Info::ATOMS::reference	ref = (*begin);
		ref->age += delta_time;
		if (ref->age < my_atm_life)
		{
			controller->update_Atom(count, ref);
			ref->pos += (ref->dir * delta_time);
			++count;
		}
		else
		{
			IF_SUCCEEDED(controller->destroy_Atom(ref))
			{
				begin = my_atms.erase(begin);
				continue;
			}
		}
		++begin;
	}

	my_create_time += delta_time;
	IF_TRUE(my_create_cycle < my_create_time)
	{
		for (DWORD count = my_per_create; count != 0; --count)
		{
			ptr_DxAtom atom;
			IF_SUCCEEDED(controller->create_Atom(&atom))
			{
				try
				{
					my_atms.push_back(atom);
				}
				catch (...)
				{
				}
			}
		}
		my_create_time -= my_create_cycle;
	}
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, set_PerCreate)(CONST DWORD num)
{
	my_per_create = num;
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, set_AtomMax)(CONST DWORD atm_max)
{
	hdx_Particle_Info::VERTEXBUFFER buffer;
	IF_SUCCEEDED(my_device->CreateVertexBuffer(
		atm_max * sizeof(hdx_Line_FVF),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		hdx_Line_FVF::FVF,
		D3DPOOL_DEFAULT,
		&buffer,
		0))
	{
		DEL_COM(my_buffer);
		my_buffer = buffer;
		my_atm_max = atm_max;
	}
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, set_CreateCycle)(CONST FLOAT time)
{
	my_create_cycle = time;
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, set_AtomLife)(CONST FLOAT time)
{
	my_atm_life = time;
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, set_AtomRadius)(CONST FLOAT scale, CONST FLOAT min, CONST FLOAT max)
{
	my_atm_radius_scale = scale;
	my_atm_radius_min = min(min, max);
	my_atm_radius_max = max(min, max);
}



CLASS_REALIZE_FUNC_T(hdx_Particle_Own, VOID, get_info)(ptr_Particle_Info info)CONST
{
	info->device = my_device;
	info->buffer = my_buffer;
	info->atms = my_atms;
	info->per_create = my_per_create;
	info->atm_max = my_atm_max;
	info->atm_life = my_atm_life;
	info->create_cycle = my_create_cycle;
	info->atm_radius_scale = my_atm_radius_scale;
	info->atm_radius_min = my_atm_radius_min;
	info->atm_radius_max = my_atm_radius_max;
}




CLASS_REALIZE_FUNC(hdx_Particle_Own, render_before)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_LIGHTING, FALSE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSIZE, (DWORD&)my_atm_radius_max));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSIZE_MIN, (DWORD&)my_atm_radius_min));
	
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE));
	
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSCALE_A, 0));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSCALE_B, 0));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSCALE_C, (DWORD&)my_atm_radius_scale));

	hr = ADD_FLAG(hr, my_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
	hr = ADD_FLAG(hr, my_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));

	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));
	return hr;
}



CLASS_REALIZE_FUNC(hdx_Particle_Own, render_after)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_LIGHTING, TRUE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE));
	return hr;
}



CLASS_REALIZE_FUNC(hdx_Particle_Own, render)(VOID)
{
	my_device->SetFVF(hdx_Line_FVF::FVF);
	my_device->SetStreamSource(0, my_buffer, 0, sizeof(hdx_Line_FVF));

	DWORD size = min(my_atms.size(), my_atm_max);

	ptr_DxLine_FVF fvf;
	HRESULT hr;
	IF_FAILED(hr = my_buffer->Lock(
		0, size * sizeof(hdx_Line_FVF),
		(LPVOID*)&fvf,
		D3DLOCK_DISCARD))
	{
		return hr;
	}

	hdx_Particle_Info::ATOMS::reverse_iterator begin = my_atms.rbegin();
	hdx_Particle_Info::ATOMS::reverse_iterator end = my_atms.rend();
	for (DWORD index = 0; index < my_atm_max; ++index)
	{
		IF_TRUE(begin == end)
		{
			break;
		}

		hdx_Particle_Info::ATOMS::reference ref = (*begin);
		fvf[index].pos = ref->pos;
		fvf[index].color = ref->color;
		++begin;
	}

	my_buffer->Unlock();

	return my_device->DrawPrimitive(
		D3DPT_POINTLIST,
		0, size);
}



CLASS_REALIZE_FUNC(hdx_Particle_Own, reset)(VOID)
{
	DEL_COM(my_device);
	DEL_COM(my_buffer);
	return S_OK;
}