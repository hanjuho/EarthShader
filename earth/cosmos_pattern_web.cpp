#include "hcosmos.h"



using namespace h;



Embedded_Cosmos::Cosmos_Web::Cosmos_Web(VOID)
: my_focus(0)
{
}



Embedded_Cosmos::Cosmos_Web::~Cosmos_Web(VOID)
{
	my_web.reset();
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Web, initialize)(hdx_3DDevice device)
{
	HRESULT hr;
	IF_FAILED(hr = my_web.initialize(device))
	{
		IF_TRUE(IS_FLAG(hr, E_HANDLE))
		{
			IF_SUCCEEDED(my_web.reset())
			{
				hr = my_web.initialize(device);
			}
		}

		IF_FAILED(hr)
		{
			return hr;
		}
	}

	return hr;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Web, render)(VOID)
{
	for (DWORD index = buffer.size() - 1; index != -1; --index)
	{
		buffer[index]->render();
	}

	DWORD option = 1;
	IF_TRUE(my_web.get_NumOfPoints() != buffer.size())
	{
		my_web.set_points(buffer.size());
		my_web.set_lines(buffer.size() - 1);

		option = ADD_FLAG(option, 2);
	}

	my_web.update(option, this);

	my_web.get_Device()->SetTransform(D3DTS_WORLD, &hdx_mNor);
	IF_SUCCEEDED(my_web.render_before())
	{
		my_web.render();
		my_web.render_after();
	}

	return S_OK;
}



CLASS_REALIZE_FUNC(Embedded_Cosmos::Cosmos_Web, focus)(DWORD focus)
{
	IF_FALSE(focus < my_web.get_NumOfPoints())
	{
		return E_INVALIDARG;
	}

	my_focus = focus;
	my_web.update(2, this);

	return S_OK;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Web, VOID, update_FVF)(DWORD index, ptr_DxLine_FVF fvf)
{
	DOUBLE pos[3];
	buffer[index]->get_Position(pos);
	fvf->pos.x = FLOAT(pos[0]);
	fvf->pos.y = FLOAT(pos[1]);
	fvf->pos.z = FLOAT(pos[2]);
	fvf->color = hdx_WHITE;
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos::Cosmos_Web, VOID, update_Line)(DWORD index, ptr_DxLine_Pair pair)
{
	DWORD mate = index < my_focus ? index : index + 1;
	pair->first = my_focus;
	pair->second = mate;
}