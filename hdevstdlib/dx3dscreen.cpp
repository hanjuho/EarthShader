#include "hdxscreen.h"
#include "hdxcamera.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_3DScreen)(VOID)
: my_d3dDevice(NULL), 
my_d2dDevice(NULL)
{
	D3DXMatrixIdentity(&my_view);
	D3DXMatrixIdentity(&my_proj);
}



CLASS_REALIZE_DESTRUCTOR(hdx_3DScreen)(VOID)
{
	destroy();
}



CLASS_REALIZE_FUNC(hdx_3DScreen, initialize)(HWND hWnd, BOOL bFullScreen)
{
	destroy();

	LPDIRECT3D9 d3d = Direct3DCreate9(DIRECT3D_VERSION);
	IF_NULL(d3d)
	{
		return E_FAIL;
	}

	HRESULT hr;
	IF_SUCCEEDED(hr = hdx_create3DDevice(
		d3d, hWnd, bFullScreen,
		&my_d3dDevice))
	{
		hr = D3DXCreateSprite(my_d3dDevice, &my_d2dDevice);
	}
	DEL_COM(d3d);

	IF_FAILED(hr)
	{		
		return destroy();
	}

	my_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	my_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	my_d3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	my_d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return hr;
}



CLASS_REALIZE_FUNC(hdx_3DScreen, destroy)(VOID)
{
	DEL_COM(my_d3dDevice);
	DEL_COM(my_d2dDevice);

	return S_OK;
}



CLASS_REALIZE_FUNC_T(hdx_3DScreen, VOID, update)(FLOAT deltaTime)
{
}



CLASS_REALIZE_FUNC(hdx_3DScreen, set_CamaraView)(ptr_F3DCamera camera)
{
	camera->get_ViewMatrix(&my_view._11);

	return my_d3dDevice->SetTransform(D3DTS_VIEW, &my_view);
}



CLASS_REALIZE_FUNC(hdx_3DScreen, set_CamaraView)(ptr_D3DCamera camera)
{
	return E_FAIL;
}



CLASS_REALIZE_FUNC(hdx_3DScreen, set_ScreenView)(CONST FLOAT fRadian, CONST FLOAT fRate, CONST FLOAT fNear, CONST FLOAT fFar)
{
	D3DXMatrixPerspectiveFovLH(&my_proj, fRadian, fRate, fNear, fFar);

	return my_d3dDevice->SetTransform(D3DTS_PROJECTION, &my_proj);
}



CLASS_REALIZE_FUNC_T(hdx_3DScreen, VOID, get_CameraView)(FLOAT view[16])CONST
{
	memcpy(view, &my_view._11, sizeof(FLOAT)* 16);
}



CLASS_REALIZE_FUNC_T(hdx_3DScreen, VOID, get_ScreenView)(FLOAT view[16])CONST
{
	memcpy(view, &my_proj._11, sizeof(FLOAT)* 16);
}



CLASS_REALIZE_FUNC_T(hdx_3DScreen, hdx_3DDevice, get_3DDevice)(VOID)
{
	return my_d3dDevice;
}



CLASS_REALIZE_FUNC_T(hdx_3DScreen, LPD3DXSPRITE, get_2DDevice)(VOID)
{
	return my_d2dDevice;
}