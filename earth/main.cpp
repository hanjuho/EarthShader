#include "hcosmos.h"
#include <hdxdefbase.h>
#include <hdxscreen.h>
#include <time.h>
#include <string>



using namespace h;



LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	srand(DWORD(time(NULL)));

	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;	

	HWND hWnd;
	IF_SUCCEEDED(h::hdx_createHwnd(WndProc, TEXT("Test"), rect.left, rect.top, rect.right, rect.bottom, 0, &hWnd))
	{
		ShowWindow(hWnd, SHOW_OPENWINDOW);

		h::hdx_3DScreen screen;
		h::hdx_MouseDevice mouse;
		h::hdx_KeyboardDevice keyboard;

		LONG sizex = 200;
		LONG sizey = 40;
		LONG gapx = 5;
		LONG gapy = 20;

		// l t r b
		RECT rects[4] =
		{
			{
				gapx,
				gapy,
				gapx + sizex, 
				gapy + sizey
			},
			{
				rect.right - sizex - gapx * 1,
				gapy, 
				rect.right - gapx, 
				gapy + sizey },
			{
				rect.right - sizex - gapx,
				gapy * 2 + sizey,
				rect.right - gapx,
				gapy * 2 + sizey *2
			},
			{
				rect.right - sizex - gapx,
				gapy * 3 + sizey * 2,
				rect.right - gapx,
				gapy * 3 + sizey * 3
			}
		};

		WCHAR			buffer[512];
		LPD3DXFONT		font;

		hdx_3DDevice	d3dDevice;
		LPD3DXSPRITE	d2dDevice;

		IF_FAILED(screen.initialize(hWnd, TRUE))
		{
			MessageBox(NULL, TEXT("CREATEFAIL_WINDOW"), TEXT("INIT"), MB_OK);
			return 0;
		}

		IF_FAILED(mouse.initialize(hWnd))
		{
			MessageBox(NULL, TEXT("CREATEFAIL_MOUSE"), TEXT("INIT"), MB_OK);
			return 0;
		}

		IF_FAILED(keyboard.initialize(hWnd))
		{
			MessageBox(NULL, TEXT("CREATEFAIL_KEYBOARD"), TEXT("INIT"), MB_OK);
			return 0;
		}

		d3dDevice = screen.get_3DDevice();
		d2dDevice = screen.get_2DDevice();

		screen.set_ScreenView(D3DX_PI / 4.0f, FLOAT(800) / FLOAT(600), 1.0f, 1000000.0f);

		D3DXCreateFont(
			d3dDevice,
			30,
			0,
			FW_DONTCARE,
			1,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			TEXT("Arial"),
			&font);

		DWORD tBegin = timeGetTime();
		DWORD tEnd = 0;
		FLOAT tDt = 0.0f;
		FLOAT elapsed = 0.0f;
		FLOAT speed = 1.0f;
		FLOAT radius = 1.0f;
		FLOAT keyDelay = 0.0f;

		Embedded_Cosmos cosmos;
		IF_FAILED(cosmos.initialize(d3dDevice))
		{
			MessageBox(NULL, TEXT("CREATEFAIL_COSMOS"), TEXT("INIT"), MB_OK);
			return 0;
		}

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			mouse.get_Signal(&cosmos, 1);
			
			IF_TRUE(keyDelay > 0.05f)
			{
				keyboard.get_Signal(&cosmos, 1);
				keyDelay -= 0.05f;
			}
			
			cosmos.update(tDt);
			cosmos.get_Container();

			screen.set_CamaraView(cosmos.get_Camera());
			d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			if (SUCCEEDED(d3dDevice->BeginScene()))
			{
				cosmos.render();

				d3dDevice->EndScene();
				IF_SUCCEEDED(d2dDevice->Begin(D3DXSPRITE_ALPHABLEND))
				{
					ZeroMemory(buffer, sizeof(TCHAR)* 512);
					swprintf_s<512>(buffer, TEXT("%f"), elapsed);
					font->DrawText(d2dDevice, buffer, -1, &rects[0], DT_LEFT, D3DCOLOR_XRGB(51, 51, 51));

					Embedded_Cosmos::ptr_SW sw = cosmos.get_FocusSW();
					ZeroMemory(buffer, sizeof(TCHAR)* 512);
					swprintf_s<512>(buffer, TEXT("%s"), sw->get_Name());
					font->DrawText(d2dDevice, buffer, -1, &rects[1], DT_LEFT, D3DCOLOR_XRGB(51, 51, 51));


					ZeroMemory(buffer, sizeof(TCHAR)* 512);
					swprintf_s<512>(buffer, TEXT("%f"), sw->get_Mass());
					font->DrawText(d2dDevice, buffer, -1, &rects[2], DT_LEFT, D3DCOLOR_XRGB(51, 51, 51));


					ZeroMemory(buffer, sizeof(TCHAR)* 512);
					swprintf_s<512>(buffer, TEXT("%f"), sw->get_Radius());
					font->DrawText(d2dDevice, buffer, -1, &rects[3], DT_LEFT, D3DCOLOR_XRGB(51, 51, 51));
					d2dDevice->End();
				}
			}
			d3dDevice->Present(NULL, NULL, NULL, NULL);

			tEnd = timeGetTime();
			tDt = FLOAT(tEnd - tBegin) * 0.001f;
			IF_TRUE(tDt > 1.0f / 10.0f)
			{
				tDt = 1.0f / 30.0f;
			}
			tBegin = tEnd;
			elapsed += tDt;
			keyDelay += tDt;
		}

		DEL_COM(font);
		mouse.destroy();
		keyboard.destroy();
		screen.destroy();
	}

	return 0;
}