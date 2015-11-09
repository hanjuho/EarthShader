#include "hdxlineweb.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_LineWeb_Own)(DWORD point_size, DWORD line_size)
: my_device(NULL), 
my_points(point_size), 
my_lines(line_size), 
my_form(D3DPT_LINELIST)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_LineWeb_Own)(VOID)
{
	reset();
}



CLASS_REALIZE_FUNC(hdx_LineWeb_Own, initialize)(CONST hdx_3DDevice device, CONST D3DPRIMITIVETYPE form)
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

	set_WebForm(form);

	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, VOID, update)(CONST DWORD option, ptr_DxLineWeb_Controller controller)
{
	IF_TRUE(IS_FLAG(option, 1))
	{
		for (DWORD index = my_points.size() - 1; index != -1; --index)
		{
			controller->update_FVF(index, &my_points[index]);
		}
	}

	IF_TRUE(IS_FLAG(option, 2))
	{
		for (DWORD index = my_lines.size() - 1; index != -1; --index)
		{
			controller->update_Line(index, &my_lines[index]);
		}
	}
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, VOID, set_WebForm)(CONST D3DPRIMITIVETYPE form)
{
	my_form = form;
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, VOID, set_points)(CONST DWORD size)
{
	my_points.resize(size);
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, VOID, set_lines)(CONST DWORD size)
{
	my_lines.resize(size);
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, VOID, get_info)(ptr_DxLineWeb_Info info)CONST
{
	info->device = my_device;
	info->points = my_points;
	info->lines = my_lines;
	info->form = my_form;
}



CLASS_REALIZE_FUNC(hdx_LineWeb_Own, render_before)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_LIGHTING, FALSE));
	hr = ADD_FLAG(hr, my_device->SetMaterial(&hdx_WHITE_MTRL));
	hr = ADD_FLAG(hr, my_device->SetTexture(0, NULL));
	return hr;
}



CLASS_REALIZE_FUNC(hdx_LineWeb_Own, render_after)(VOID)
{
	HRESULT hr = S_OK;
	hr = ADD_FLAG(hr, my_device->SetMaterial(&hdx_BLACK_MTRL));
	hr = ADD_FLAG(hr, my_device->SetRenderState(D3DRS_LIGHTING, TRUE));
	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_LineWeb_Own, render)(VOID)
{
	my_device->SetFVF(hdx_Line_FVF::FVF);
	return my_device->DrawIndexedPrimitiveUP(
		my_form,
		0,
		my_points.size(),
		my_lines.size(),
		&my_lines[0],
		D3DFMT_INDEX32,
		&my_points[0],
		sizeof(hdx_Line_FVF));
}



CLASS_REALIZE_FUNC(hdx_LineWeb_Own, reset)(VOID)
{
	DEL_COM(my_device);
	set_points(0);
	set_lines(0);
	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, DWORD, get_NumOfPoints)(VOID)CONST
{
	return my_points.size();
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, DWORD, get_NumOfLines)(VOID)CONST
{
	return my_lines.size();
}



CLASS_REALIZE_FUNC_T(hdx_LineWeb_Own, hdx_3DDevice, get_Device)(VOID)
{
	return my_device;
}