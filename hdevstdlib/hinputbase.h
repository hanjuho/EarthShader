#pragma once



#include "hdefbase.h"



namespace h
{
	DLL_DECL_INTERFACE(h_MouseEvent)
	{
		INTERFACE_DECL_FUNC_T(LONG, get_X)		(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(LONG, get_Y)		(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(LONG, get_W)		(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(LONG, get_AbsX)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(LONG, get_AbsY)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(BOOL, is_RButton)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(BOOL, is_LButton)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(BOOL, is_WButton)	(VOID)CONST PURE;
	} *ptr_MouseEvent;



	DLL_DECL_INTERFACE(h_MouseListener)
	{
		INTERFACE_DECL_FUNC_T(VOID, on_ClickDown)	(ptr_MouseEvent e)PURE;
		INTERFACE_DECL_FUNC_T(VOID, on_ClickUp)		(ptr_MouseEvent e)PURE;
		INTERFACE_DECL_FUNC_T(VOID, on_Drag)		(ptr_MouseEvent e)PURE;
		INTERFACE_DECL_FUNC_T(VOID, on_Move)		(ptr_MouseEvent e)PURE;
		INTERFACE_DECL_FUNC_T(VOID, on_Wheel)		(ptr_MouseEvent e)PURE;
	} *ptr_MouseListener;



	DLL_DECL_INTERFACE(h_MouseDevice)
	{
		INTERFACE_DECL_FUNC(get_Signal)(ptr_MouseListener pBuffer, CONST DWORD size)PURE;
	} *ptr_MouseDevice;



	DLL_DECL_INTERFACE(h_KeyboardEvent)
	{
		INTERFACE_DECL_FUNC_T(BOOL, is_KeyDown)(BYTE key)CONST PURE;
	} *ptr_KeyboardEvent;



	DLL_DECL_INTERFACE(h_KeyboardListener)
	{
		INTERFACE_DECL_FUNC_T(VOID, on_KeyState)(ptr_KeyboardEvent key)PURE;
	} *ptr_KeyboardListener;



	DLL_DECL_INTERFACE(h_KeyboardDevice)
	{
		INTERFACE_DECL_FUNC(get_Signal)(ptr_KeyboardListener pBuffer, CONST DWORD size)PURE;
	} *ptr_KeyboardDevice;
};