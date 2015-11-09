#include "hcosmos.h"



using namespace h;



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, camera_initialize)(ptr_SW focus)
{
	IF_NULL(focus)
	{
		return;
	}

	IF_TRUE(my_focusSW == focus)
	{ 
		return;
	}

	DEL_SW(my_focusSW);
	my_focusSW = focus;
	my_focusSW->addRef();

	my_length = FLOAT(my_focusSW->get_Radius()) * 7.0f;

	camera_Look();
}


CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, camera_Look)(VOID)
{
	DOUBLE pos[3];
	my_focusSW->get_Position(pos);

	my_camera.get_ZDir(my_dir);

	D3DXVECTOR3 position(
		FLOAT(pos[0]) - (my_dir.x * my_length),
		FLOAT(pos[1]) - (my_dir.y * my_length),
		FLOAT(pos[2]) - (my_dir.z * my_length));

	my_camera.set_Position(position);
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, camera_Rotate)(CONST FLOAT x, CONST FLOAT y)
{
	D3DXVECTOR3 dir(1.0f, 1.0f, 1.0f);
	IF_TRUE(x)
	{
		my_camera.rotate_YAxis(x * HDX_ANGLE);
	}

	IF_TRUE(y)
	{
		my_camera.rotate_XAxis(y * HDX_ANGLE);
	}
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, camera_Zoom)(CONST FLOAT z)
{
	my_length += FLOAT(my_focusSW->get_Radius()) * 1.5f * (abs(z) / z);

	my_length = max(FLOAT(my_focusSW->get_Radius()) * 3.0f, my_length);
}