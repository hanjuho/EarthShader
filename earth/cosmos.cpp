#include "hcosmos.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(Cosmos)(
	LPCWSTR name, DWORD type, DOUBLE radius, DOUBLE mass,
	DOUBLE avx, DOUBLE avy, DOUBLE avz,
	DOUBLE vx, DOUBLE vy, DOUBLE vz ,
	DOUBLE x, DOUBLE y, DOUBLE z,
	DOUBLE t, DOUBLE rx, DOUBLE ry, DOUBLE rz)
	: name(name), type(type), radius(radius), mass(mass), torque(t), tangle(0.0)
{
	hdx_set3DVector(acc_velocity, avx, avy, avz);
	hdx_set3DVector(velocity, vx, vy, vz);
	hdx_set3DVector(position, x, y, z);
	hdx_set3DVector(tAxis, rx, ry, rz);
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Sun)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Sun");
	co->mass = 332946.0;
	co->radius = 109.0;
	
	co_setVector(co->velocity, 0.0, 0.0, 0.0);
	
	co->torque = atan2l(co->radius * 0.5, 7189.0 * METER_UNIT);
	co_setVector(co->tAxis, hdx_vNorY.x, hdx_vNorY.y, hdx_vNorY.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Mercury)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Mercury");
	co->mass = 0.055;
	co->radius = 0.383;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 3.86f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 126072 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 2.64f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 6.52 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Venus)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Venus");
	co->mass = 0.815;
	co->radius = 0.949;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 3.38f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 170496 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 0.01f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 10.892 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Earth)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Earth");
	co->mass = 1.0;
	co->radius = 1.0;
	
	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 7.25f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 107218.8 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 23.439f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 465.11 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Moon)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Moon");
	co->mass = 0.0123;
	co->radius = 0.271;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 7.25f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = (110898 ) * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 1.5424f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 16.655 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Mars)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Mars");
	co->mass = 0.531;
	co->radius = 0.533;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 5.65f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 110898 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 25.19f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 868.22 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Jupiter)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Jupiter");
	co->mass = 317.8;
	co->radius = 11.209;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 6.09f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 86677.2 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 3.13f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 45.360 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Saturn)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Saturn");
	co->mass = 95.162;
	co->radius = 9.449;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 5.51f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 34700.4 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 26.73f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 35500 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Uranus)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Uranus");
	co->mass = 14.536;
	co->radius = 4.007;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 6.48f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 24462 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 97.77f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 9320 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(ptr_Cosmos, h::co_get_Neptune)(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	co->name = TEXT("Neptune");
	co->mass = 17.147;
	co->radius = 3.883;

	D3DXMATRIX	RX;
	D3DXVECTOR3	DIR;

	D3DXMatrixRotationX(&RX, HDX_ANGLE * 6.43f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorZ, &RX);
	DOUBLE velocity = 19555.2 * METER_UNIT;
	co_setVector(co->velocity, DIR.x * velocity, DIR.y * velocity, DIR.z * velocity);

	D3DXMatrixRotationZ(&RX, HDX_ANGLE * 28.32f);
	D3DXVec3TransformNormal(&DIR, &hdx_vNorY, &RX);
	co->torque = atan2l(co->radius * 0.5, 9660 * METER_UNIT);
	co_setVector(co->tAxis, DIR.x, DIR.y, DIR.z);
	co_setVector(co->position, x, y, z);

	return co;
}



REALIZE_FUNC_T(DOUBLE *, h::co_setVector)(DOUBLE vector[3], CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;

	return vector;
}



REALIZE_FUNC(h::co_init_Planet)(hdx_3DDevice device, LPCWSTR filename, ptr_DxMesh pOut)
{
	HRESULT hr;
	IF_FAILED(hr = pOut->initialize(device))
	{
		IF_TRUE(IS_FLAG(hr, E_HANDLE))
		{
			IF_SUCCEEDED(pOut->reset())
			{
				hr = pOut->initialize(device);
			}
		}

		IF_FAILED(hr)
		{
			return hr;
		}
	}


	LPD3DXMESH d3dMesh;
	IF_FAILED(hr = hdx_init_MappedSphere(device, 1.0f, 30, 30, &d3dMesh))
	{		
		return hr;
	}

	pOut->set_NumOfMaterial(1);
	pOut->set_Material(0, &hdx_WHITE_MTRL);
	pOut->load_Texture(0, filename);
	pOut->set_Mesh(d3dMesh);

	return hr;
}