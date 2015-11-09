#include "hcosmos.h"



using namespace h;



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_KeyState)(ptr_KeyboardEvent key)
{
	for (BYTE index = DIK_1; index <= DIK_0; ++index)
	{
		DWORD i = index - 2;
		IF_FALSE(i < my_buffer.buffer.size())
		{
			return;
		}

		IF_TRUE(key->is_KeyDown(index))
		{
			camera_initialize(my_buffer.buffer[i]);

			my_buffer.focus(i);
		}
	}

	IF_TRUE(key->is_KeyDown(DIK_UP))
	{
		my_timeAccel += 0.1f;

		my_timeAccel = min(100.0f, my_timeAccel);
	}

	IF_TRUE(key->is_KeyDown(DIK_DOWN))
	{
		my_timeAccel -= 0.1f;

		my_timeAccel = max(1.0f, my_timeAccel);
	}

	IF_TRUE(key->is_KeyDown(DIK_LEFT))
	{
		DOUBLE mass = my_focusSW->get_Mass();

		mass = max(0.001, mass - mass * 0.1);

		my_focusSW->set_Mass(mass);
	}

	IF_TRUE(key->is_KeyDown(DIK_RIGHT))
	{
		DOUBLE mass = my_focusSW->get_Mass();

		mass += mass * 0.1;

		my_focusSW->set_Mass(mass);
	}
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_ClickDown)(ptr_MouseEvent e)
{ 
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_ClickUp)(ptr_MouseEvent e)
{

}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_Drag)(ptr_MouseEvent e)
{
	camera_Rotate(FLOAT(e->get_X()), FLOAT(e->get_Y()));
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_Move)(ptr_MouseEvent e)
{
}



CLASS_REALIZE_FUNC_T(Embedded_Cosmos, VOID, on_Wheel)(ptr_MouseEvent e)
{
	camera_Zoom(FLOAT(e->get_W()));
}