#include "hcosmos.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(Embedded_Cosmos)(VOID)
: my_container(&my_factory, &my_buffer),
my_finder(&my_buffer),
my_device(NULL),
my_focusSW(NULL),
my_dir(hdx_vNorY),
my_length(1.0f),
my_timeAccel(1.0f)
{
}



CLASS_REALIZE_DESTRUCTOR(Embedded_Cosmos)(VOID)
{
	DEL_COM(my_device);
	DEL_SW(my_focusSW);
}



CLASS_REALIZE_FUNC(Embedded_Cosmos, initialize)(hdx_3DDevice device)
{
	IF_NULL(device)
	{
		return E_INVALIDARG;
	}

	HRESULT hr;
	IF_FAILED(hr = my_factory.initialize(device)) 
	{
		return hr;
	}

	IF_FAILED(hr = my_buffer.initialize(device))
	{
		return hr;
	}

	Embedded_Cosmos::ptr_Container container;
	ptr_SW sw;
	container = this->get_Container();
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 0, &sw));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 1, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 2, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 3, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 4, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 5, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 6, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 7, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 8, NULL));
	hr = ADD_FLAG(hr, container->push(TEXT("Planet"), 9, NULL));

	IF_FAILED(hr)
	{
		return hr;
	}

	camera_initialize(sw);

	my_device = device;
	my_device->AddRef();

	return hr;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos, render)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_buffer.render());
	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, update)(FLOAT delta)
{
	delta = delta * my_timeAccel;

	static 	DOUBLE aBuffer[3], bBuffer[3], cBuffer[3];

	SW_Buffer & buffer = my_buffer.buffer;
	for (LONG A = buffer.size() - 1; A != -1; --A)
	{
		for (LONG B = A - 1; B != -1; --B)
		{
			buffer[A]->get_Position(aBuffer);
			buffer[B]->get_Position(bBuffer);

			D3DXVECTOR3 vec;
			vec.x = FLOAT(bBuffer[0] - aBuffer[0]);
			vec.y = FLOAT(bBuffer[1] - aBuffer[1]);
			vec.z = FLOAT(bBuffer[2] - aBuffer[2]);

			DOUBLE distance = D3DXVec3Length(&vec);
			DOUBLE g = (G_EARTH) / (distance * distance);

			D3DXVec3Normalize(&vec, &vec);

			cBuffer[0] = vec.x * g;
			cBuffer[1] = vec.y * g;
			cBuffer[2] = vec.z * g;

			buffer[A]->get_accVelocity(aBuffer);
			aBuffer[0] = aBuffer[0] + (cBuffer[0] * buffer[B]->get_Mass());
			aBuffer[1] = aBuffer[1] + (cBuffer[1] * buffer[B]->get_Mass());
			aBuffer[2] = aBuffer[2] + (cBuffer[2] * buffer[B]->get_Mass());
			buffer[A]->set_accVelocity(aBuffer);

			buffer[B]->get_accVelocity(bBuffer);
			bBuffer[0] = bBuffer[0] + (-cBuffer[0] * buffer[A]->get_Mass());
			bBuffer[1] = bBuffer[1] + (-cBuffer[1] * buffer[A]->get_Mass());
			bBuffer[2] = bBuffer[2] + (-cBuffer[2] * buffer[A]->get_Mass());
			buffer[B]->set_accVelocity(bBuffer);
		}

		buffer[A]->update(delta);
	}

	camera_Look();
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, ptr_F3DCamera, get_Camera)(VOID)CONST
{
	return ptr_F3DCamera(&my_camera);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, Embedded_Cosmos::ptr_Factory, get_Factory)(VOID)CONST
{
	return (ptr_Factory)&my_factory;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, Embedded_Cosmos::ptr_Container, get_Container)(VOID)CONST
{
	return (ptr_Container)&my_container;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, Embedded_Cosmos::ptr_Finder, get_Finder)(VOID)CONST
{
	return (ptr_Finder)&my_finder;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, Embedded_Cosmos::ptr_SW, get_FocusSW)(VOID)CONST
{
	return (ptr_SW)my_focusSW;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, Embedded_Cosmos::iterator, create_iterator)(VOID)CONST
{
	return iterator((ptr_SW_Buffer)&my_buffer);
}