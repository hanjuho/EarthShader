#include "hdxinput.h"
#include <algorithm>



using namespace h;



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, LONG, get_X)(VOID)CONST
{
	return lX;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, LONG, get_Y)(VOID)CONST
{
	return lY;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, LONG, get_W)(VOID)CONST
{
	return lZ;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, LONG, get_AbsX)(VOID)CONST
{
	return lAbsX;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, LONG, get_AbsY)(VOID)CONST
{
	return lAbsY;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, BOOL, is_RButton)(VOID)CONST
{
	return rgbButtons[0] & 0x80;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, BOOL, is_LButton)(VOID)CONST
{
	return rgbButtons[1] & 0x80;
}



CLASS_REALIZE_FUNC_T(hdx_MouseEvent, BOOL, is_WButton)(VOID)CONST
{
	return rgbButtons[2] & 0x80;
}



CLASS_REALIZE_CONSTRUCTOR(hdx_MouseDevice)(VOID)
: my_diMouse(NULL),
my_bClickBefore(FALSE),
my_bClickCurrent(FALSE),
my_bMoveCurrent(FALSE),
my_dwEventState(0)
{
	ZeroMemory(&my_diEvent.lX, 28);
}



CLASS_REALIZE_DESTRUCTOR(hdx_MouseDevice)(VOID)
{
	destroy();
}



CLASS_REALIZE_FUNC(hdx_MouseDevice, initialize)(HWND hWnd)
{
	IF_NOT_NULL(my_diMouse)
	{
		return S_FALSE;
	}

	LPDIRECTINPUT8 di = NULL;
	HRESULT hr;
	IF_FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di, NULL))
	{
		return hr;
	}

	IF_NOT_NULL(hWnd)
	{
		hr = hdx_createInputMouse(di, hWnd, ADD_FLAG(DISCL_NONEXCLUSIVE, DISCL_FOREGROUND), &my_diMouse);
	}
		else
		{
			hr = hdx_createInputMouse(di, hWnd, ADD_FLAG(DISCL_NONEXCLUSIVE, DISCL_BACKGROUND), &my_diMouse);
		}

		DEL_COM(di);

		return hr;
}



CLASS_REALIZE_FUNC(hdx_MouseDevice, destroy)(VOID)
{
	DEL_COM(my_diMouse);

	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_MouseDevice, get_Signal)(ptr_MouseListener pBuffer, CONST DWORD size)
{
	IF_NULL(my_diMouse)
	{
		return E_FAIL;
	}

	ZeroMemory(&my_diEvent.lX, 28);
	HRESULT hr = my_diMouse->GetDeviceState(20, &my_diEvent.lX);
	GetCursorPos(LPPOINT(&my_diEvent.lAbsX));
	IF_FAILED(hr)
	{
		hr = my_diMouse->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = my_diMouse->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			return hr;

		hr = my_diMouse->GetDeviceState(20, &my_diEvent.lX);
	}

	IF_SUCCEEDED(hr)
	{
		my_bClickCurrent =
			my_diEvent.rgbButtons[0] |
			my_diEvent.rgbButtons[1] |
			my_diEvent.rgbButtons[2];

		my_bMoveCurrent = my_diEvent.lX | my_diEvent.lY;

		IF_TRUE(my_bClickBefore & my_bClickCurrent)
		{
			IF_TRUE(my_bMoveCurrent)
			{
				// click drag
				my_dwEventState = ADD_FLAG(my_dwEventState, 16);
			}
		}
		else
		{
			IF_TRUE(my_bClickCurrent)
			{
				// click up
				my_dwEventState = ADD_FLAG(my_dwEventState, 2);
			}
			else
			{
				IF_TRUE(my_bClickBefore)
				{
					// click down
					my_dwEventState = ADD_FLAG(my_dwEventState, 1);
				}
				else
				{
					// move
					IF_TRUE(my_bMoveCurrent)
					{
						my_dwEventState = ADD_FLAG(my_dwEventState, 4);
					}
				}
			}
		}
		// wheel
		IF_TRUE(my_diEvent.lZ)
		{
			my_dwEventState = ADD_FLAG(my_dwEventState, 8);
		}

		ptr_MouseListener mouselistener;
		for (LONG index = size - 1; index != -1; --index)
		{
			mouselistener = (pBuffer + index);
			IF_TRUE(IS_FLAG(my_dwEventState, 1))
			{
				mouselistener->on_ClickDown(&my_diEvent);
			}

			IF_TRUE(IS_FLAG(my_dwEventState, 2))
			{
				mouselistener->on_ClickUp(&my_diEvent);
			}

			IF_TRUE(IS_FLAG(my_dwEventState, 4))
			{
				mouselistener->on_Move(&my_diEvent);
			}

			IF_TRUE(IS_FLAG(my_dwEventState, 8))
			{
				mouselistener->on_Wheel(&my_diEvent);
			}

			IF_TRUE(IS_FLAG(my_dwEventState, 16))
			{
				mouselistener->on_Drag(&my_diEvent);
			}
		}
		my_bClickBefore = my_bClickCurrent;
		my_dwEventState = 0;
	}

	return hr;
}