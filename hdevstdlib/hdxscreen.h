#pragma once



#include "hdxdefbase.h"
#include "houtputbase.h"



namespace h
{
	DLL_DECL_INHERITANCE(CLASS, hdx_3DScreen, h_3DScreen)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_3DScreen)				(VOID);
		INTERFACE_DECL_FUNC(initialize)					(HWND hWnd, BOOL bFullScreen);
		INTERFACE_DECL_FUNC(destroy)					(VOID);
		INTERFACE_DECL_FUNC_T(VOID, update)				(FLOAT deltaTime);
		INTERFACE_DECL_FUNC(set_CamaraView)				(ptr_F3DCamera camera);
		INTERFACE_DECL_FUNC(set_CamaraView)				(ptr_D3DCamera camera);
		INTERFACE_DECL_FUNC(set_ScreenView)				(CONST FLOAT fRadian, CONST FLOAT fRate, CONST FLOAT fNear, CONST FLOAT fFar);
		INTERFACE_DECL_FUNC_T(VOID, get_CameraView)		(FLOAT view[16])CONST;
		INTERFACE_DECL_FUNC_T(VOID, get_ScreenView)		(FLOAT view[16])CONST;
		CLASS_DECL_FUNC_T(hdx_3DDevice, get_3DDevice)	(VOID);
		CLASS_DECL_FUNC_T(LPD3DXSPRITE, get_2DDevice)	(VOID);
		
	private:
		hdx_3DDevice	my_d3dDevice;
		LPD3DXSPRITE	my_d2dDevice;
		D3DXMATRIX		my_view;
		D3DXMATRIX		my_proj;
	} *ptr_DX3DScreen;
};