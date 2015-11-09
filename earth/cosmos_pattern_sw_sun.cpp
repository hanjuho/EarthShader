#include "hcosmos.h"



using namespace h;



float randomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}



Embedded_Cosmos::Star_SW::Star_SW(DWORD id)
:Cosmos_SW(id)
{
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Star_SW, initialize)(ptr_Cosmos hw, ptr_Cosmos_Planet planet)
{
	HRESULT hr = Cosmos_SW::initialize(hw, planet);
	IF_FAILED(hr)
	{
		return hr;
	}

	DOUBLE * buffer = hw->position;
	my_light = hdx_light_Point(
		&D3DXVECTOR3(
		FLOAT(buffer[0]), 
		FLOAT(buffer[1]), 
		FLOAT(buffer[2])), 
		&hdx_WHITE);

	my_light.Range = 10000000.0f;

	hdx_3DDevice device;
	device = planet->get_Device();
	device->LightEnable(0, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_AMBIENT, 0);
	return hr;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Star_SW, render)(VOID)
{
	my_light.Position.x = FLOAT(my_cosmos->position[0]);
	my_light.Position.y = FLOAT(my_cosmos->position[1]);
	my_light.Position.z = FLOAT(my_cosmos->position[2]);

	hdx_3DDevice device = my_planet->get_Device();
	device->SetLight(my_light.channel, &my_light);
	device->LightEnable(my_light.channel, TRUE);

	return Cosmos_SW::render();;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Star_SW, create_Atom)(ptr_DxAtom * atom)
{
	ptr_DxAtom atm = NULL;
	try
	{
		atm = new hdx_Atom();
		atm->pos.x = randomFloat(FLOAT(-my_cosmos->radius / 2), FLOAT(my_cosmos->radius / 2));
		atm->pos.y = randomFloat(FLOAT(-my_cosmos->radius / 2), FLOAT(my_cosmos->radius / 2));
		atm->pos.z = randomFloat(FLOAT(-my_cosmos->radius / 2), FLOAT(my_cosmos->radius / 2));
		atm->dir.x = FLOAT(atm->pos.x - my_cosmos->position[0]);
		atm->dir.y = FLOAT(atm->pos.y - my_cosmos->position[1]);
		atm->dir.z = FLOAT(atm->pos.z - my_cosmos->position[2]);
		D3DXVec3Normalize(&atm->dir, &atm->dir);
		atm->dir *= 0.1f;
		atm->color = hdx_RED;
		atm->age = 0.0f;
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}

	(*atom) = atm;

	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Star_SW, VOID, update_Atom)(DWORD index, ptr_DxAtom atom)
{
}