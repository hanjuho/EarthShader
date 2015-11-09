#include "hdxinput.h"
#include <algorithm>



using namespace h;



CLASS_REALIZE_FUNC_T(hdx_KeyboardEvent, BOOL, is_KeyDown)(BYTE key)CONST
{
	return buffer[key] & 0x80;
}



CLASS_REALIZE_CONSTRUCTOR(hdx_KeyboardDevice)(VOID)
: my_diKeyboard(NULL),
my_bEditMode(FALSE),
my_text(TEXT("")),
my_focus(0)
{
	ZeroMemory(my_diEvent.buffer, sizeof(my_diEvent.buffer));
}



CLASS_REALIZE_DESTRUCTOR(hdx_KeyboardDevice)(VOID)
{
	destroy();
}



CLASS_REALIZE_FUNC(hdx_KeyboardDevice, initialize)(HWND hWnd)
{
	IF_NOT_NULL(my_diKeyboard)
	{
		return S_FALSE;
	}

	LPDIRECTINPUT8 di = NULL;
	HRESULT hr;
	IF_SUCCEEDED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di, NULL))
	{
		IF_NOT_NULL(hWnd)
		{
			hr = hdx_createInputKeyborad(di, hWnd, ADD_FLAG(DISCL_NONEXCLUSIVE, DISCL_FOREGROUND), &my_diKeyboard);
		}
		else
		{
			hr = hdx_createInputKeyborad(di, hWnd, ADD_FLAG(DISCL_NONEXCLUSIVE, DISCL_BACKGROUND), &my_diKeyboard);
		}
	}
	DEL_COM(di);

	return hr;
}



CLASS_REALIZE_FUNC(hdx_KeyboardDevice, destroy)(VOID)
{
	DEL_COM(my_diKeyboard);

	return S_OK;
}



CLASS_REALIZE_FUNC(hdx_KeyboardDevice, get_Signal)(ptr_KeyboardListener pBuffer, DWORD size)
{
	IF_NULL(my_diKeyboard)
	{
		return E_FAIL;
	}

	ZeroMemory(&my_diEvent.buffer, 256);
	HRESULT hr = my_diKeyboard->GetDeviceState(256, &my_diEvent.buffer);
	IF_FAILED(hr)
	{
		hr = my_diKeyboard->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = my_diKeyboard->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			return hr;

		hr = my_diKeyboard->GetDeviceState(256, &my_diEvent.buffer);
	}

	IF_SUCCEEDED(hr)
	{
		BYTE * buffer = my_diEvent.buffer;
		IF_TRUE(my_bEditMode)
		{
			BOOL alt = FALSE, ctrl = FALSE, shift = FALSE;
			IF_SIGNAL(buffer[DIK_LALT])
			{
				alt = TRUE;
			}

			IF_SIGNAL(buffer[DIK_LCONTROL])
			{
				ctrl = TRUE;
			}

			IF_SIGNAL(buffer[DIK_LSHIFT])
			{
				shift = TRUE;
			}			

			IF_NONSIGNAL(alt)
			{
				LONG index;
				for (index = 11; index != 1; --index)
				{
					IF_SIGNAL(buffer[index])
					{
						switch (index)
						{
						case DIK_0:my_text.push_back(TEXT('0' ^ (shift * 25))); break;
						case DIK_1:my_text.push_back(TEXT('1' ^ (shift * 16))); break;
						case DIK_2:my_text.push_back(TEXT('2' ^ (shift * 114))); break;
						case DIK_3:my_text.push_back(TEXT('3' ^ (shift * 16))); break;
						case DIK_4:my_text.push_back(TEXT('4' ^ (shift * 16))); break;
						case DIK_5:my_text.push_back(TEXT('5' ^ (shift * 16))); break;
						case DIK_6:my_text.push_back(TEXT('6' ^ (shift * 104))); break;
						case DIK_7:my_text.push_back(TEXT('7' ^ (shift * 15))); break;
						case DIK_8:my_text.push_back(TEXT('8' ^ (shift * 16))); break;
						case DIK_9:my_text.push_back(TEXT('9' ^ (shift * 16))); break;
						}
					}
				}

				IF_NONSIGNAL((shift & ctrl))
				{
					IF_SIGNAL(buffer[DIK_BACKSPACE])
					{
						IF_FALSE(my_text.empty())
						{
							my_text.pop_back();
						}
					}
				}
			}
			else
			{

			}

			for (LONG index = 255; index != -1; --index)
			{
				IF_SIGNAL(buffer[index])
				{
					int a = 0;
				}
			}
		}
		else
		{
			for (LONG index = 255; index != -1; --index)
			{
				IF_SIGNAL(buffer[index])
				{
					for (LONG index = size - 1; index != -1; --index)
					{
						(pBuffer + index)->on_KeyState(&my_diEvent);
					}
					break;
				}
			}
		}
	}

	return hr;
}



CLASS_REALIZE_FUNC_T(hdx_KeyboardDevice, VOID, set_EditMode)(BOOL bTrue)
{
	IF_TRUE(my_bEditMode != bTrue)
	{
		my_bEditMode = bTrue;
		my_text = TEXT("");
	}
}



CLASS_REALIZE_FUNC_T(hdx_KeyboardDevice, LPCWSTR, get_EditText)(VOID)
{
	return my_text.c_str();
}