#pragma once



#include "hdefbase.h"



namespace h
{
	DLL_DECL_INTERFACE(h_Renderer)
	{
		INTERFACE_DECL_FUNC(render_before)	(VOID)PURE;
		INTERFACE_DECL_FUNC(render_after)	(VOID)PURE;
		INTERFACE_DECL_FUNC(render)			(VOID)PURE;
		INTERFACE_DECL_FUNC(reset)			(VOID)PURE;
	} *ptr_Renderer;

	DLL_DECL_INTERFACE(h_3DScreen)
	{
		INTERFACE_DECL_FUNC_T(VOID, update)			(FLOAT deltaTime)PURE;
		INTERFACE_DECL_FUNC(set_CamaraView)			(ptr_F3DCamera camera)PURE;
		INTERFACE_DECL_FUNC(set_CamaraView)			(ptr_D3DCamera camera)PURE;
		INTERFACE_DECL_FUNC(set_ScreenView)			(CONST FLOAT fRadian, CONST FLOAT fRate, CONST FLOAT fNear, CONST FLOAT fFar)PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_CameraView)	(FLOAT view[16])CONST PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_ScreenView)	(FLOAT view[16])CONST PURE;
	} *ptr_3DScreen;
};