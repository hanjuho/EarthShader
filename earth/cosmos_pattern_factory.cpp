#include "hcosmos.h"



using namespace h;



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Factory, DWORD, open_Create_ID)(VOID)
{
	static DWORD id = 0;
	return ++id;
}



Embedded_Cosmos::Cosmos_Factory::Cosmos_Factory(VOID)
: my_planets(10)
{
}



Embedded_Cosmos::Cosmos_Factory::~Cosmos_Factory(VOID)
{
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Factory, initialize)(hdx_3DDevice device)
{
	IF_NULL(device)
	{
		return E_INVALIDARG;
	}

	LPCWSTR planets[10] =
	{
		TEXT("..\\resource/sun.jpg"),
		TEXT("..\\resource/mercury.jpg"),
		TEXT("..\\resource/venus.jpg"),
		TEXT("..\\resource/earth.jpg"),
		TEXT("..\\resource/moon.jpg"),
		TEXT("..\\resource/mars.jpg"),
		TEXT("..\\resource/jupiter.jpg"),
		TEXT("..\\resource/saturn.jpg"),
		TEXT("..\\resource/uranus.jpg"),
		TEXT("..\\resource/neptune.jpg"),
	};
	
	HRESULT hr;
	for (LONG index = my_planets.size() - 1; index != -1; --index)
	{
		IF_FAILED(hr = my_planets[index].initialize(device, planets[index]))
		{
			return hr;
		}
	}

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Factory, construct_hw)(CONST KEY key, CONST DWORD option, DWORD * id, ptr_HW hw)CONST
{
	(*id) = open_Create_ID();

	hw->type = option;
	switch (hw->type)
	{
	case 0:
		co_get_Sun(hw, 0.0, 0.0, 0.0);
		break;
	case 1:
		co_get_Mercury(hw, 57909176.0 * METER_UNIT, 0.0, 0.0);
		break;
	case 2:
		co_get_Venus(hw, 108208926.0 * METER_UNIT, 0.0, 0.0);
		break;
	case 3:
		co_get_Earth(hw, 149597887.5  * METER_UNIT, 0.0, 0.0);
		break;
	case 4:
		co_get_Moon(hw, 149982287.5 * METER_UNIT, 0.0, 0.0);
		break;
	case 5:
		co_get_Mars(hw, 227936637 * METER_UNIT, 0.0, 0.0);
		break;
	case 6:
		co_get_Jupiter(hw, 816081455 * METER_UNIT, 0.0, 0.0);
		break;
	case 7:
		co_get_Saturn(hw, 1426725413 * METER_UNIT, 0.0, 0.0);
		break;
	case 8:
		co_get_Uranus(hw, 2870972220 * METER_UNIT, 0.0, 0.0);
		break;
	case 9:
		co_get_Neptune(hw, 4498252900 * METER_UNIT, 0.0, 0.0);
		break;
	default:
		hw->type = 0;
		co_get_Sun(hw, 0.0, 0.0, 0.0);
		break;
	}

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Factory, destruct_hw)(CONST KEY key, CONST DWORD id, ptr_HW hw)CONST
{
	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Factory, develop_sw)(CONST ptr_HW hw, CONST DWORD id, ptr_SW * pSw)CONST
{
	ptr_Cosmos_SW cosmos_sw;
	try
	{
		switch (hw->type)
		{
		case 0:
		{
				  cosmos_sw = new Star_SW(id);
				  break;
		}
		case 3:
		{
				  cosmos_sw = new Earth_SW(id);
				  break;
		}
		default:
		{
				   cosmos_sw = new Cosmos_SW(id);
				   break;
		}
		}
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}

	IF_FAILED(cosmos_sw->initialize(hw, ptr_Cosmos_Planet(&my_planets[hw->type])))
	{
		DEL_POINTER(cosmos_sw);

		return E_FAIL;
	}

	(*pSw) = cosmos_sw;

	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Factory, DWORD, create_id)(CONST KEY key, CONST DWORD option)CONST
{
	return open_Create_ID();
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Factory, DWORD, get_type)(VOID)CONST
{
	return 0;
}