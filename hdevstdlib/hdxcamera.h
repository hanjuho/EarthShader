#pragma once



#include "hdxdefbase.h"
#include "hcamera.h"



namespace h
{
	DLL_DECL_INHERITANCE(CLASS, hdx_F3DCamera, h_F3DCamera)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_F3DCamera)			(VOID);
		INTERFACE_DECL_FUNC_T(VOID, multiply)		(CONST FLOAT mMatrix[16]);
		INTERFACE_DECL_FUNC_T(VOID, move_XDir)		(CONST FLOAT value);
		INTERFACE_DECL_FUNC_T(VOID, move_YDir)		(CONST FLOAT value);
		INTERFACE_DECL_FUNC_T(VOID, move_ZDir)		(CONST FLOAT value);
		INTERFACE_DECL_FUNC_T(VOID, rotate_XAxis)	(CONST FLOAT angle);
		INTERFACE_DECL_FUNC_T(VOID, rotate_YAxis)	(CONST FLOAT angle);
		INTERFACE_DECL_FUNC_T(VOID, rotate_ZAxis)	(CONST FLOAT angle);
		INTERFACE_DECL_FUNC_T(VOID, get_XDir)		(FLOAT vXdir[3])CONST;
		INTERFACE_DECL_FUNC_T(VOID, get_YDir)		(FLOAT vYdir[3])CONST;
		INTERFACE_DECL_FUNC_T(VOID, get_ZDir)		(FLOAT vZdir[3])CONST;
		INTERFACE_DECL_FUNC_T(VOID, get_Position)	(FLOAT vPos[3])CONST;
		INTERFACE_DECL_FUNC_T(VOID, set_Position)	(CONST FLOAT vPos[3]);
		INTERFACE_DECL_FUNC_T(VOID, set_Target)		(CONST FLOAT vPos[3]);
		INTERFACE_DECL_FUNC_T(VOID, get_ViewMatrix)	(FLOAT mView[16])CONST;
	private:
		D3DXVECTOR3	my_vPos;
		D3DXVECTOR3	my_vXDir;
		D3DXVECTOR3	my_vYDir;
		D3DXVECTOR3	my_vZDir;
	} *ptr_DXF3DCamera;
};