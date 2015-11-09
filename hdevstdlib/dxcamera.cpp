#include "hdxcamera.h"



using namespace h;



CLASS_REALIZE_CONSTRUCTOR(hdx_F3DCamera)(VOID)
: my_vPos(0.0f, 0.0f, 0.0f),
my_vXDir(hdx_vNorX),
my_vYDir(hdx_vNorY),
my_vZDir(hdx_vNorZ)
{
}



CLASS_REALIZE_DESTRUCTOR(hdx_F3DCamera)(VOID)
{
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, multiply)(CONST FLOAT mMatrix[16])
{
	D3DXVec3TransformNormal(&my_vXDir, &my_vXDir, LPD3DXMATRIX(mMatrix));
	D3DXVec3TransformNormal(&my_vYDir, &my_vYDir, LPD3DXMATRIX(mMatrix));
	D3DXVec3TransformNormal(&my_vZDir, &my_vZDir, LPD3DXMATRIX(mMatrix));
	D3DXVec3TransformCoord(&my_vPos, &my_vPos, LPD3DXMATRIX(mMatrix));
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, move_XDir)(CONST FLOAT value)
{
	my_vPos += my_vXDir * value;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, move_YDir)(CONST FLOAT value)
{
	my_vPos += my_vYDir * value;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, move_ZDir)(CONST FLOAT value)
{
	my_vPos += my_vZDir * value;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, rotate_XAxis)(CONST FLOAT angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &my_vXDir, angle);

	D3DXVec3TransformCoord(&my_vYDir, &my_vYDir, &T);
	D3DXVec3TransformCoord(&my_vZDir, &my_vZDir, &T);
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, rotate_YAxis)(CONST FLOAT angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &my_vYDir, angle);

	D3DXVec3TransformCoord(&my_vXDir, &my_vXDir, &T);
	D3DXVec3TransformCoord(&my_vZDir, &my_vZDir, &T);
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, rotate_ZAxis)(CONST FLOAT angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &my_vZDir, angle);

	D3DXVec3TransformCoord(&my_vXDir, &my_vXDir, &T);
	D3DXVec3TransformCoord(&my_vYDir, &my_vYDir, &T);
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, get_XDir)(FLOAT vXdir[3])CONST
{
	vXdir[0] = my_vXDir.x;
	vXdir[1] = my_vXDir.y;
	vXdir[2] = my_vXDir.z;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, get_YDir)(FLOAT vYdir[3])CONST
{
	vYdir[0] = my_vYDir.x;
	vYdir[1] = my_vYDir.y;
	vYdir[2] = my_vYDir.z;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, get_ZDir)(FLOAT vZdir[3])CONST
{
	vZdir[0] = my_vZDir.x;
	vZdir[1] = my_vZDir.y;
	vZdir[2] = my_vZDir.z;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, get_Position)(FLOAT vPos[3])CONST
{
	vPos[0] = my_vPos.x;
	vPos[1] = my_vPos.y;
	vPos[2] = my_vPos.z;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, set_Position)(CONST FLOAT vPos[3])
{
	my_vPos.x = vPos[0];
	my_vPos.y = vPos[1];
	my_vPos.z = vPos[2];
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, set_Target)(CONST FLOAT vPos[3])
{
	// 받아온 값의 노말을 계산
	D3DXVECTOR3 nor = D3DXVECTOR3(vPos);
	D3DXVec3Normalize(&nor, &nor);

	D3DXVec3Cross(&my_vYDir, &nor, &my_vZDir);
	D3DXVec3Normalize(&my_vYDir, &my_vYDir);

	D3DXVec3Cross(&my_vXDir, &nor, &my_vYDir);
	D3DXVec3Normalize(&my_vXDir, &my_vXDir);

	my_vZDir = nor;
}



CLASS_REALIZE_FUNC_T(hdx_F3DCamera, VOID, get_ViewMatrix)(FLOAT mView[16])CONST
{
	float x = -D3DXVec3Dot(&my_vXDir, &my_vPos);
	float y = -D3DXVec3Dot(&my_vYDir, &my_vPos);
	float z = -D3DXVec3Dot(&my_vZDir, &my_vPos);

	mView[0] = my_vXDir.x;	mView[1] = my_vYDir.x;	mView[2] = my_vZDir.x;	mView[3] = 0.0f;
	mView[4] = my_vXDir.y;	mView[5] = my_vYDir.y;	mView[6] = my_vZDir.y;	mView[7] = 0.0f;
	mView[8] = my_vXDir.z;	mView[9] = my_vYDir.z;	mView[10] = my_vZDir.z; mView[11] = 0.0f;
	mView[12] = x;			mView[13] = y;			mView[14] = z;			mView[15] = 1.0f;
}