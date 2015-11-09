#pragma once



#define DIRECTINPUT_VERSION 0x0800



#include "hdefbase.h"
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>



#define CAST_M4X4(f)	reinterpret_cast<D3DXMATRIX*>(f)
#define CAST_V3(f)		reinterpret_cast<D3DXVECTOR3*>(f)
#define HDX_ANGLE		D3DX_PI / 180



namespace h
{
	typedef	IDirect3DDevice9 *		hdx_3DDevice;
	typedef IDirectInputDevice8 *	hdx_InputDevice;

	DLL_DECL_STRUCT(hdx_MappedMesh_FVF)
	{
		static CONST DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		FLOAT tu;
		FLOAT tv;
	} *ptr_DxMappedMesh_FVF;

	DLL_DECL_STRUCT(hdx_Line_FVF)
	{
		static CONST DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
		D3DXVECTOR3 pos;
		DWORD color;
	} *ptr_DxLine_FVF;

	DLL_DECL_INHERITANCE(STRUCT, hdx_Light, D3DLIGHT9)
	{
		DWORD channel;
	} *ptr_DXLight;

	DLL_DECL_STRUCT(hdx_TextRect)
	{
		typedef std::wstring TEXT;
		TEXT	text;
		HFONT	font;
		RECT	rect;
		UINT	format;
	} *ptr_DxTextRect;

	DLL_DECL_FUNC(hdx_load_Texture)			(hdx_3DDevice device, LPCWSTR filename, LPDIRECT3DTEXTURE9 * ppOut);
	DLL_DECL_FUNC(hdx_load_TextureEx)		(hdx_3DDevice device, LPCWSTR filename, LPDIRECT3DTEXTURE9 * ppOut);
	DLL_DECL_FUNC(hdx_texture_DrawText)		(LPDIRECT3DTEXTURE9 texture, CONST ptr_DxTextRect rect);
	DLL_DECL_FUNC(hdx_texture_DrawRect)		(LPDIRECT3DTEXTURE9 texture, CONST RECT * rect, HBRUSH brush);
	DLL_DECL_FUNC(hdx_init_MappedSphere)	(hdx_3DDevice device, FLOAT radius, UINT slices, UINT stacks, LPD3DXMESH * ppOut);
	DLL_DECL_FUNC(hdx_init_MappedPane)		(hdx_3DDevice device, DWORD row, DWORD col, FLOAT gap, LPD3DXMESH * ppOut, BOOL bz = FALSE);
	DLL_DECL_FUNC(hdx_set3DVector)			(FLOAT vec[3], CONST FLOAT x, CONST FLOAT y, CONST FLOAT z);
	DLL_DECL_FUNC(hdx_set3DVector)			(DOUBLE vec[3], CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);

	DLL_DECL_FUNC_T(DWORD, hdx_create_LightChannel)		(VOID);
	DLL_DECL_FUNC_T(hdx_Light, hdx_light_Directional)	(CONST D3DXVECTOR3 * direction, CONST D3DXCOLOR * color);
	DLL_DECL_FUNC_T(hdx_Light, hdx_light_Point)			(CONST D3DXVECTOR3 * position, CONST D3DXCOLOR * color);
	DLL_DECL_FUNC_T(hdx_Light, hdx_light_Spot)			(CONST D3DXVECTOR3 * position, CONST D3DXVECTOR3 * direction, CONST D3DXCOLOR * color);

	DLL_DECL_FUNC_T(D3DMATERIAL9, hdx_mtrl)(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	static CONST D3DXCOLOR hdx_WHITE(D3DCOLOR_XRGB(255, 255, 255));
	static CONST D3DXCOLOR hdx_BLACK(D3DCOLOR_XRGB(0, 0, 0));
	static CONST D3DXCOLOR hdx_RED(D3DCOLOR_XRGB(255, 0, 0));
	static CONST D3DXCOLOR hdx_GREEN(D3DCOLOR_XRGB(0, 255, 0));
	static CONST D3DXCOLOR hdx_BLUE(D3DCOLOR_XRGB(0, 0, 255));
	static CONST D3DXCOLOR hdx_YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	static CONST D3DXCOLOR hdx_CYAN(D3DCOLOR_XRGB(0, 255, 255));
	static CONST D3DXCOLOR hdx_MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	static CONST D3DMATERIAL9 hdx_WHITE_MTRL = hdx_mtrl(hdx_WHITE, hdx_WHITE, hdx_WHITE, hdx_BLACK, 2.0f);
	static CONST D3DMATERIAL9 hdx_BLACK_MTRL = hdx_mtrl(hdx_BLACK, hdx_BLACK, hdx_BLACK, hdx_BLACK, 2.0f);
	static CONST D3DMATERIAL9 hdx_RED_MTRL = hdx_mtrl(hdx_RED, hdx_RED, hdx_RED, hdx_BLACK, 2.0f);
	static CONST D3DMATERIAL9 hdx_GREEN_MTRL = hdx_mtrl(hdx_GREEN, hdx_GREEN, hdx_GREEN, hdx_BLACK, 2.0f);
	static CONST D3DMATERIAL9 hdx_BLUE_MTRL = hdx_mtrl(hdx_BLUE, hdx_BLUE, hdx_BLUE, hdx_BLACK, 2.0f);
	static CONST D3DMATERIAL9 hdx_YELLOW_MTRL = hdx_mtrl(hdx_YELLOW, hdx_YELLOW, hdx_YELLOW, hdx_BLACK, 2.0f);

	static CONST D3DXVECTOR3 hdx_vNorX(1.0f, 0.0f, 0.0f);
	static CONST D3DXVECTOR3 hdx_vNorY(0.0f, 1.0f, 0.0f);
	static CONST D3DXVECTOR3 hdx_vNorZ(0.0f, 0.0f, 1.0f);

	static CONST D3DXMATRIX	hdx_mNor(
		1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1.0f);

	DLL_DECL_FUNC(hdx_createHwnd)			(WNDPROC wndProc, LPCWSTR title, LONG x, LONG y, LONG width, LONG height, LONG toolbar, HWND * ppOut);
	DLL_DECL_FUNC(hdx_create3DDevice)		(LPDIRECT3D9 d3d, HWND hWnd, BOOL bFull, hdx_3DDevice * ppOut);
	DLL_DECL_FUNC(hdx_createInputMouse)		(LPDIRECTINPUT8 di, HWND hWnd, DWORD level, hdx_InputDevice * ppOut);
	DLL_DECL_FUNC(hdx_createInputKeyborad)	(LPDIRECTINPUT8 di, HWND hWnd, DWORD level, hdx_InputDevice * ppOut);

	typedef std::map<std::wstring, D3DXHANDLE> SHADER_MAP;
	DLL_DECL_STRUCT(hdx_PixelShader)
	{
		typedef LPD3DXCONSTANTTABLE		TABLE;
		typedef LPDIRECT3DPIXELSHADER9	SHADER;
		CLASS_DECL_STRUCTOR(hdx_PixelShader)	(VOID);
		INTERFACE_DECL_FUNC(initialize)			(hdx_3DDevice device, LPCWSTR script, LPCSTR entry = "PSMain", LPCSTR version = "ps_2_0");
		INTERFACE_DECL_FUNC(begin)				(VOID);
		INTERFACE_DECL_FUNC(end)				(VOID);
		hdx_3DDevice	device;
		TABLE			table;
		SHADER			shader;
	} *ptr_DxPixelShader;

	DLL_DECL_STRUCT(hdx_VertexShader)
	{
		typedef LPD3DXCONSTANTTABLE		TABLE;
		typedef LPDIRECT3DVERTEXSHADER9	SHADER;
		CLASS_DECL_STRUCTOR(hdx_VertexShader)	(VOID);
		INTERFACE_DECL_FUNC(initialize)			(hdx_3DDevice device, LPCWSTR script, LPCSTR entry = "VSMain", LPCSTR version = "vs_3_0");
		INTERFACE_DECL_FUNC(begin)				(VOID);
		INTERFACE_DECL_FUNC(end)				(VOID);
		hdx_3DDevice	device;
		TABLE			table;
		SHADER			shader;
	} *ptr_DxVertexShader;

	DLL_DECL_INTERFACE(hdx_Particle_Controller)	*ptr_DxParticle_Controller;
	DLL_DECL_INTERFACE(hdx_Particle)			*ptr_DxParticle;
	DLL_DECL_INTERFACE(hdx_LineWeb_Controller)	*ptr_DxLineWeb_Controller;
	DLL_DECL_INTERFACE(hdx_LineWeb)				*ptr_DxLineWeb;
	DLL_DECL_INTERFACE(hdx_Material)			*ptr_DxMaterial;
	DLL_DECL_INTERFACE(hdx_Mesh)				*ptr_DxMesh;
};
