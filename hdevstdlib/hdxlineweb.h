#pragma once



#include "hdxdefbase.h"
#include "houtputbase.h"



namespace h
{
	DLL_DECL_STRUCT(hdx_Line_Pair)
	{
		DWORD first;
		DWORD second;
	} *ptr_DxLine_Pair;

	DLL_DECL_STRUCT(hdx_LineWeb_Info)
	{
		typedef std::vector<hdx_Line_FVF>	POINTS;
		typedef std::vector<hdx_Line_Pair>	PAIRS;
		typedef D3DPRIMITIVETYPE			FORM;
		hdx_3DDevice	device;
		POINTS			points;
		PAIRS			lines;
		FORM			form;
	} *ptr_DxLineWeb_Info;

	DLL_BASE_INHERITANCE(hdx_LineWeb, h_Renderer)
	{
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST D3DPRIMITIVETYPE form = D3DPT_LINELIST)PURE;
		INTERFACE_DECL_FUNC_T(VOID, update)			(CONST DWORD option, ptr_DxLineWeb_Controller controller)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_WebForm)	(CONST D3DPRIMITIVETYPE form)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_points)		(CONST DWORD size)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_lines)		(CONST DWORD size)PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_DxLineWeb_Info info)CONST PURE;
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfPoints)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfLines)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)	(VOID)PURE;
	} *ptr_DxLineWeb;

	DLL_DECL_INTERFACE(hdx_LineWeb_Controller)
	{
		INTERFACE_DECL_FUNC_T(VOID, update_FVF)		(DWORD index, ptr_DxLine_FVF fvf)PURE;
		INTERFACE_DECL_FUNC_T(VOID, update_Line)	(DWORD index, ptr_DxLine_Pair pair)PURE;
	} *ptr_DxLineWeb_Controller;

	DLL_DECL_INHERITANCE(CLASS, hdx_LineWeb_Own, hdx_LineWeb)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_LineWeb_Own)		(DWORD point_size = 0, DWORD line_size = 0);
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST D3DPRIMITIVETYPE form = D3DPT_LINELIST);
		INTERFACE_DECL_FUNC_T(VOID, update)			(CONST DWORD option, ptr_DxLineWeb_Controller controller);
		INTERFACE_DECL_FUNC_T(VOID, set_WebForm)	(CONST D3DPRIMITIVETYPE form);
		INTERFACE_DECL_FUNC_T(VOID, set_points)		(CONST DWORD size);
		INTERFACE_DECL_FUNC_T(VOID, set_lines)		(CONST DWORD size);
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_DxLineWeb_Info info)CONST;
		INTERFACE_DECL_FUNC(render_before)			(VOID);
		INTERFACE_DECL_FUNC(render_after)			(VOID);
		INTERFACE_DECL_FUNC(render)					(VOID);
		INTERFACE_DECL_FUNC(reset)					(VOID);
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfPoints)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(DWORD, get_NumOfLines)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)	(VOID);
	private:
		hdx_3DDevice				my_device;
		hdx_LineWeb_Info::POINTS	my_points;
		hdx_LineWeb_Info::PAIRS		my_lines;
		hdx_LineWeb_Info::FORM		my_form;
	} *ptr_DxLinWeb_Own;
};