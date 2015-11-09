#include "hcosmos.h"



using namespace h;



Cosmos DEFAULT_COSMOS;



Embedded_Cosmos::Cosmos_SW::Cosmos_SW(DWORD id)
: SW(id), my_cosmos(&DEFAULT_COSMOS), my_planet(NULL)
{
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_SW, initialize)(ptr_Cosmos hw, ptr_Cosmos_Planet planet)
{
	IF_NULL((hw && planet))
	{
		return E_INVALIDARG;
	}

	my_cosmos = hw;
	my_planet = planet;

	my_planet->set_CreateCycle(10.0f);
	my_planet->set_AtomRadius(0.1f, 10.0f);
	my_planet->set_AtomLife(1000.0f);

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_SW, invalid)(VOID)
{
	my_cosmos = &DEFAULT_COSMOS;

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_SW, render)(VOID)
{
	return ADD_FLAG(my_planet->render_mesh(), my_planet->render_particle());
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, update)(FLOAT delta)
{
	my_cosmos->tangle += (HDX_ANGLE * my_cosmos->torque * delta);

	my_cosmos->velocity[0] = my_cosmos->velocity[0] + my_cosmos->acc_velocity[0] * delta;
	my_cosmos->velocity[1] = my_cosmos->velocity[1] + my_cosmos->acc_velocity[1] * delta;
	my_cosmos->velocity[2] = my_cosmos->velocity[2] + my_cosmos->acc_velocity[2] * delta;

	my_cosmos->position[0] = my_cosmos->position[0] + my_cosmos->velocity[0] * delta;
	my_cosmos->position[1] = my_cosmos->position[1] + my_cosmos->velocity[1] * delta;
	my_cosmos->position[2] = my_cosmos->position[2] + my_cosmos->velocity[2] * delta;

	my_cosmos->acc_velocity[0] = 0.0;
	my_cosmos->acc_velocity[1] = 0.0;
	my_cosmos->acc_velocity[2] = 0.0;

	DOUBLE * vec = my_cosmos->position;
	D3DXMatrixTranslation(&T, FLOAT(vec[0]), FLOAT(vec[1]), FLOAT(vec[2]));

	FLOAT scale = FLOAT(my_cosmos->radius);
	IF_TRUE(my_cosmos->type != 0)
	{
		scale *= 1.0f;
	}
	D3DXMatrixScaling(&S, scale, scale, scale);

	vec = my_cosmos->tAxis;
	D3DXMatrixRotationAxis(&R, &D3DXVECTOR3(FLOAT(vec[0]), FLOAT(vec[1]), FLOAT(vec[2])), FLOAT(my_cosmos->tangle));

	D3DXMatrixMultiply(&M, &S, &T);
	D3DXMatrixMultiply(&my_planet->my_form, &R, &M);

	my_planet->update(delta, this);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Name)(LPCWSTR name)
{
	my_cosmos->name = name;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Radius)(DOUBLE radius)
{
	my_cosmos->radius = radius;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Mass)(DOUBLE mass)
{
	my_cosmos->mass = mass;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Torque)(DOUBLE torque)
{
	my_cosmos->torque = torque;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_accVelocity)(CONST DOUBLE buffer[3])
{
	my_cosmos->acc_velocity[0] = buffer[0];
	my_cosmos->acc_velocity[1] = buffer[1];
	my_cosmos->acc_velocity[2] = buffer[2];
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Velocity)(CONST DOUBLE buffer[3])
{
	my_cosmos->velocity[0] = buffer[0];
	my_cosmos->velocity[1] = buffer[1];
	my_cosmos->velocity[2] = buffer[2];
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_Position)(CONST DOUBLE buffer[3])
{
	my_cosmos->position[0] = buffer[0];
	my_cosmos->position[1] = buffer[1];
	my_cosmos->position[2] = buffer[2];
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, set_TAxis)(CONST DOUBLE buffer[3])
{
	my_cosmos->tAxis[0] = buffer[0];
	my_cosmos->tAxis[1] = buffer[1];
	my_cosmos->tAxis[2] = buffer[2];
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, LPCWSTR, get_Name)(VOID)CONST
{
	return my_cosmos->name.c_str();
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DWORD, get_Type)(VOID)CONST
{
	return my_cosmos->type;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE, get_Radius)(VOID)CONST
{
	return my_cosmos->radius;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE, get_Mass)(VOID)CONST
{
	return my_cosmos->mass;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE *, get_accVelocity)(DOUBLE buffer[3])CONST
{	
	buffer[0] = my_cosmos->acc_velocity[0];
	buffer[1] = my_cosmos->acc_velocity[1];
	buffer[2] = my_cosmos->acc_velocity[2];

	return buffer;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE *, get_Velocity)(DOUBLE buffer[3])CONST
{
	buffer[0] = my_cosmos->velocity[0];
	buffer[1] = my_cosmos->velocity[1];
	buffer[2] = my_cosmos->velocity[2];

	return buffer;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE *, get_Position)(DOUBLE buffer[3])CONST
{
	buffer[0] = my_cosmos->position[0];
	buffer[1] = my_cosmos->position[1];
	buffer[2] = my_cosmos->position[2];

	return buffer;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE, get_Torque)(VOID)CONST
{
	return my_cosmos->torque;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, DOUBLE *, get_TAxis)(DOUBLE buffer[3])CONST
{
	buffer[0] = my_cosmos->tAxis[0];
	buffer[1] = my_cosmos->tAxis[1];
	buffer[2] = my_cosmos->tAxis[2];

	return buffer;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_SW, create_Atom)(ptr_DxAtom * atom)
{
	ptr_DxAtom atm = NULL;
	try
	{
		atm = new hdx_Atom();
		atm->pos.x = FLOAT(my_cosmos->position[0] - my_cosmos->velocity[0] * my_cosmos->radius);
		atm->pos.y = FLOAT(my_cosmos->position[1] - my_cosmos->velocity[1] * my_cosmos->radius);
		atm->pos.z = FLOAT(my_cosmos->position[2] - my_cosmos->velocity[2] * my_cosmos->radius);
		atm->dir.x = 0.0f;
		atm->dir.y = 0.0f;
		atm->dir.z = 0.0f;
		atm->color = hdx_WHITE;
		atm->age = 0.0f;
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}	
	
	(*atom) = atm;

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_SW, destroy_Atom)(ptr_DxAtom atom)
{
	delete atom;
	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_SW, VOID, update_Atom)(DWORD index, ptr_DxAtom atom)
{
}