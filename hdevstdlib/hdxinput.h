#pragma once



#include "hdxdefbase.h"
#include "hinputbase.h"
#include <string>



namespace h
{
	DLL_DECL_INHERITANCE(STRUCT, hdx_MouseEvent, h_MouseEvent)
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
		BYTE    rgbButtons[8];
		LONG	lAbsX;
		LONG	lAbsY;
		INTERFACE_DECL_FUNC_T(LONG, get_X)		(VOID)CONST;
		INTERFACE_DECL_FUNC_T(LONG, get_Y)		(VOID)CONST;
		INTERFACE_DECL_FUNC_T(LONG, get_W)		(VOID)CONST;
		INTERFACE_DECL_FUNC_T(LONG, get_AbsX)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(LONG, get_AbsY)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(BOOL, is_RButton)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(BOOL, is_LButton)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(BOOL, is_WButton)	(VOID)CONST;
	} *ptr_DXMouseEvent;



	DLL_DECL_INHERITANCE(CLASS, hdx_MouseDevice, h_MouseDevice)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_MouseDevice)	(VOID);
		INTERFACE_DECL_FUNC(initialize)	(HWND hWnd);
		INTERFACE_DECL_FUNC(destroy)	(VOID);
		INTERFACE_DECL_FUNC(get_Signal)	(ptr_MouseListener pBuffer, CONST DWORD size);
	private:
		hdx_InputDevice	my_diMouse;
		hdx_MouseEvent	my_diEvent;
		BOOL			my_bClickBefore;
		BOOL			my_bClickCurrent;
		BOOL			my_bMoveCurrent;
		DWORD			my_dwEventState;
	} *ptr_DXMouse;



	DLL_DECL_INHERITANCE(STRUCT, hdx_KeyboardEvent, h_KeyboardEvent)
	{
		BYTE buffer[256];
		INTERFACE_DECL_FUNC_T(BOOL, is_KeyDown)(BYTE key)CONST;
	} *ptr_DXKeyboardEvent;



	DLL_DECL_INHERITANCE(CLASS, hdx_KeyboardDevice, h_KeyboardDevice)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_KeyboardDevice)		(VOID);
		INTERFACE_DECL_FUNC(initialize)				(HWND hWnd);
		INTERFACE_DECL_FUNC(destroy)				(VOID);
		INTERFACE_DECL_FUNC(get_Signal)				(ptr_KeyboardListener pBuffer, CONST DWORD size);
		INTERFACE_DECL_FUNC_T(VOID, set_EditMode)	(BOOL bTrue);
		INTERFACE_DECL_FUNC_T(LPCWSTR, get_EditText)(VOID);
	private:
		hdx_InputDevice		my_diKeyboard;
		hdx_KeyboardEvent	my_diEvent;
		BOOL				my_bEditMode;
		std::wstring		my_text;
		LONG				my_focus;
	} *hdx_DXKeyboardEvent;
};