#include "hdxdefbase.h"



using namespace h;



REALIZE_FUNC(h::hdx_load_Texture)(hdx_3DDevice device, LPCWSTR filename, LPDIRECT3DTEXTURE9 * ppOut)
{
	LPDIRECT3DTEXTURE9 texture;
	HRESULT hr;
	IF_FAILED(hr = D3DXCreateTextureFromFile(device, filename, &texture))
	{
		TCHAR _directory[256];
		_tcprintf(_directory, "..\\%s", filename);
		IF_FAILED(hr = D3DXCreateTextureFromFile(device, _directory, &texture))
		{
			return hr;
		}
	}
	(*ppOut) = texture;

	return hr;
}



REALIZE_FUNC(h::hdx_texture_DrawText)(LPDIRECT3DTEXTURE9 texture, CONST ptr_DxTextRect rect)
{
	HRESULT hr;

	LPDIRECT3DSURFACE9 surface;
	IF_FAILED(hr = texture->GetSurfaceLevel(0, &surface))
	{
		return hr;
	}

	D3DLOCKED_RECT lockrect;
	IF_FAILED(hr = surface->LockRect(&lockrect, NULL, D3DLOCK_DISCARD))
	{
		DEL_COM(surface);
		return hr;
	}

	HDC hdc;
	IF_FAILED(hr = surface->GetDC(&hdc))
	{
		surface->UnlockRect();
		DEL_COM(surface);
		return hr;
	}

	HGDIOBJ oldFont = SelectObject(hdc, rect->font);
	LONG oldMode = SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, rect->text.c_str(), -1, &rect->rect, rect->format);
	SetBkMode(hdc, oldMode);
	SelectObject(hdc, oldFont);

	surface->UnlockRect();
	DEL_COM(surface);

	return S_OK;
}



REALIZE_FUNC(h::hdx_texture_DrawRect)(LPDIRECT3DTEXTURE9 texture, CONST RECT * rect, HBRUSH brush)
{
	HRESULT hr;

	LPDIRECT3DSURFACE9 surface;
	IF_FAILED(hr = texture->GetSurfaceLevel(0, &surface))
	{
		return hr;
	}

	D3DLOCKED_RECT lockrect;
	IF_FAILED(surface->LockRect(&lockrect, NULL, D3DLOCK_DISCARD))
	{
		DEL_COM(surface);
		return hr;
	}

	HDC hdc;
	IF_FAILED(surface->GetDC(&hdc))
	{
		surface->UnlockRect();
		DEL_COM(surface);
		return hr;
	}

	FillRect(hdc, rect, brush);

	surface->UnlockRect();
	DEL_COM(surface);

	return S_OK;
}



REALIZE_FUNC(h::hdx_init_MappedSphere)(hdx_3DDevice device, FLOAT radius, UINT slices, UINT stacks, LPD3DXMESH * ppOut)
{
	LPD3DXMESH d3dMesh = NULL;
	HRESULT hr;
	IF_FAILED(hr = D3DXCreateSphere(device, radius, slices, stacks, &d3dMesh, NULL))
	{
		return hr;
	}

	LPD3DXMESH d3dTexMesh = NULL;
	IF_FAILED(hr = d3dMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, hdx_MappedMesh_FVF::FVF, device, &d3dTexMesh))
	{
		DEL_COM(d3dMesh);
		return hr;
	}
	DEL_COM(d3dMesh);
	
	LPDWORD attributeBuffer;
	IF_SUCCEEDED(d3dTexMesh->LockAttributeBuffer(0, &attributeBuffer))
	{
		for (DWORD index = d3dTexMesh->GetNumFaces() - 1; index != -1; --index)
		{
			attributeBuffer[index] = 0;
		}
		d3dTexMesh->UnlockAttributeBuffer();
	}

	std::vector<DWORD> adjacencyBuffer(d3dTexMesh->GetNumFaces() * 3);
	IF_SUCCEEDED(d3dTexMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]))
	{
		hr = d3dTexMesh->OptimizeInplace(
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT |
			D3DXMESHOPT_VERTEXCACHE,
			&adjacencyBuffer[0],
			0, 0, 0);
	}
	
	FLOAT angle_mul = (D3DX_PI * 2) / (D3DX_PI * 2 - (D3DX_PI * 2 / slices));
	FLOAT x;
	FLOAT z;
	FLOAT len;
	FLOAT angle;
	FLOAT cos;
	FLOAT sin;

	ptr_DxMappedMesh_FVF fvf;
	IF_SUCCEEDED(hr = d3dTexMesh->LockVertexBuffer(0, (LPVOID*)&fvf))
	{
		try
		{
			LONG numVerts = d3dTexMesh->GetNumVertices();
			for (LONG i = 0; i < numVerts; i++)
			{
				x = fvf[i].nor[0];
				z = fvf[i].nor[2];

				len = D3DXVec2Length(&D3DXVECTOR2(fvf[i].pos.x, fvf[i].pos.z));
				angle = atan2f(z, x) * angle_mul;

				cos = cosf(angle);
				fvf[i].pos.x = (cos > 0.999f ? 1 : cos)  * (len);
				sin = sinf(angle);
				fvf[i].pos.z = sin * (len);

				fvf[i].nor.x = cos;
				fvf[i].nor.z = sin;

				fvf[i].tu = angle / (D3DX_PI * 2);
				fvf[i].tv = (fvf[i].nor.y / -2) + 0.5f;
			}
			d3dTexMesh->UnlockVertexBuffer();
		}
		catch (...)
		{
			MessageBox(NULL, TEXT("CREATEFAIL_PLANET"), TEXT("INIT"), MB_OK);
		}
	}

	(*ppOut) = d3dTexMesh;

	return hr;
}



REALIZE_FUNC(h::hdx_init_MappedPane)(hdx_3DDevice device, DWORD row, DWORD col, FLOAT gap, LPD3DXMESH * ppOut, BOOL bz)
{
	LPD3DXMESH d3dMesh;
	HRESULT hr;

	row = max(1, row);
	col = max(1, col);

	DWORD v_row = row + 1;
	DWORD v_col = col + 1;

	DWORD numOfFaces = row * col * 2;
	DWORD numOfVertexs = v_row * v_col;

	IF_FAILED(hr = D3DXCreateMeshFVF(
		numOfFaces,
		numOfVertexs,
		D3DXMESH_MANAGED,
		h::hdx_MappedMesh_FVF::FVF,
		device,
		&d3dMesh))
	{
		return hr;
	}

	ptr_DxMappedMesh_FVF fvf;
	IF_SUCCEEDED(d3dMesh->LockVertexBuffer(0, (void**)&fvf))
	{
		FLOAT width = row * gap;
		FLOAT depth = col * gap;

		FLOAT tu = 1.0f / FLOAT(row);
		FLOAT tv = 1.0f / FLOAT(col);

		FLOAT startX = -width / 2.0f;
		FLOAT startZ = depth / 2.0f;

		FLOAT endX = width / 2.0f;
		FLOAT endZ = -depth / 2.0f;

		LONG A = 0;
		for (FLOAT z = startZ; z >= endZ; z -= gap)
		{
			LONG B = 0;
			for (FLOAT x = startX; x <= endX; x += gap)
			{
				LONG index = A * v_row + B;

				if (bz)
				{
					h::hdx_set3DVector(fvf[index].pos, x, 0, z);
					h::hdx_set3DVector(fvf[index].nor, 0, 1, 0);
				}
				else
				{
					h::hdx_set3DVector(fvf[index].pos, x, z, 0);
					h::hdx_set3DVector(fvf[index].nor, 0, 0, -1);
				}

				fvf[index].tu = B * tu;
				fvf[index].tv = A * tv;

				++B;
			}
			++A;
		}
		d3dMesh->UnlockVertexBuffer();
	}

	LPWORD indices;
	IF_SUCCEEDED(d3dMesh->LockIndexBuffer(0, (void**)&indices))
	{
		DWORD index = 0;

		// loop through and compute the triangles of each quad
		for (DWORD A = 0; A < col; ++A)
		{
			for (DWORD B = 0; B < row; ++B)
			{
				indices[index] = WORD(A * v_row + B);
				indices[index + 1] = WORD(A * v_row + (B + 1));
				indices[index + 2] = WORD((A + 1) * v_row + B);

				indices[index + 3] = WORD((A + 1) * v_row + B);
				indices[index + 4] = WORD(A * v_row + (B + 1));
				indices[index + 5] = WORD((A + 1) * v_row + B + 1);

				// next quad
				index += 6;
			}
		}
		d3dMesh->UnlockIndexBuffer();
	}

	LPDWORD attributeBuffer;
	IF_SUCCEEDED(d3dMesh->LockAttributeBuffer(0, &attributeBuffer))
	{
		for (DWORD index = numOfFaces - 1; index != -1; --index)
		{
			attributeBuffer[index] = 0;
		}
		d3dMesh->UnlockAttributeBuffer();
	}

	std::vector<DWORD> adjacencyBuffer(d3dMesh->GetNumFaces() * 3);
	IF_SUCCEEDED(d3dMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]))
	{
		hr = d3dMesh->OptimizeInplace(
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT |
			D3DXMESHOPT_VERTEXCACHE,
			&adjacencyBuffer[0],
			0, 0, 0);
	}

	(*ppOut) = d3dMesh;

	return hr;
}



REALIZE_FUNC(h::hdx_set3DVector)(FLOAT vec[3], CONST FLOAT x, CONST FLOAT y, CONST FLOAT z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;

	return S_OK;
}



REALIZE_FUNC(h::hdx_set3DVector)(DOUBLE vec[3], CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;

	return S_OK;
}



REALIZE_FUNC_T(DWORD, h::hdx_create_LightChannel)(VOID)
{
	static DWORD channel = 0;
	return channel++;
}



REALIZE_FUNC_T(hdx_Light, h::hdx_light_Directional)(CONST D3DXVECTOR3 * direction, CONST D3DXCOLOR * color)
{
	hdx_Light light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	D3DXVec3Normalize(CAST_V3(&light.Direction), direction);
	light.channel = hdx_create_LightChannel();

	return light;
}



REALIZE_FUNC_T(hdx_Light, h::hdx_light_Point)(CONST D3DXVECTOR3 * position, CONST D3DXCOLOR * color)
{
	hdx_Light light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.channel = hdx_create_LightChannel();

	return light;
}



REALIZE_FUNC_T(hdx_Light, h::hdx_light_Spot)(CONST D3DXVECTOR3 * position, CONST D3DXVECTOR3 * direction, CONST D3DXCOLOR * color)
{
	hdx_Light light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.5f;
	light.Phi = 0.7f;
	light.channel = hdx_create_LightChannel();

	return light;
}



REALIZE_FUNC_T(D3DMATERIAL9, h::hdx_mtrl)(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, FLOAT p)
{
	D3DMATERIAL9 mtrl;

	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;

	return mtrl;
}



REALIZE_FUNC(h::hdx_createHwnd)(WNDPROC wndProc, LPCWSTR title, LONG x, LONG y, LONG width, LONG height, LONG toolbar, HWND * ppOut)
{
	IF_NULL(ppOut)
	{
		return E_INVALIDARG;
	}

	*ppOut = NULL;

	WNDCLASSEX wndclassex;

	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_CLASSDC;
	wndclassex.lpfnWndProc = wndProc != NULL ? wndProc : DefWindowProc;
	wndclassex.cbClsExtra = 0L;
	wndclassex.cbWndExtra = 0L;
	wndclassex.hInstance = GetModuleHandle(NULL);
	wndclassex.hIcon = NULL;
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hbrBackground = NULL;
	wndclassex.lpszMenuName = MAKEINTRESOURCE(toolbar);
	wndclassex.lpszClassName = TEXT("HDXSYS");
	wndclassex.hIconSm = NULL;

	RegisterClassEx(&wndclassex);

	HWND hWnd = CreateWindow(
		TEXT("HDXSYS"),
		title,
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	UnregisterClass(TEXT("HDXSYS"), GetModuleHandle(NULL));

	IF_NULL(hWnd)
	{
		return E_FAIL;
	}

	*ppOut = hWnd;

	return S_OK;
}



REALIZE_FUNC(h::hdx_create3DDevice)(LPDIRECT3D9 d3d, HWND hWnd, BOOL bFull, hdx_3DDevice * ppOut)
{
	IF_NULL(ppOut && d3d)
	{
		return E_INVALIDARG;
	}

	*ppOut = NULL;

	HRESULT hr;

	D3DDISPLAYMODE d3ddm;
	IF_FAILED(hr = d3d->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&d3ddm))
	{
		return hr;
	}

	IF_FAILED(hr = d3d->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3ddm.Format,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		D3DFMT_D16))
	{
		IF_TRUE(hr == D3DERR_NOTAVAILABLE)
		{
			return hr;
		}
	}

	D3DCAPS9 d3dCaps;
	IF_FAILED(hr = d3d->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&d3dCaps))
	{
		return hr;
	}

	DWORD dwBehaviorFlags = 0;
	IF_TRUE(d3dCaps.VertexProcessingCaps != 0)
	{
		dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hdx_3DDevice d3dDevice;
	IF_FAILED(hr = d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		dwBehaviorFlags,
		&d3dpp,
		&d3dDevice))
	{
		return hr;
	}

	*ppOut = d3dDevice;

	return S_OK;
}



REALIZE_FUNC(h::hdx_createInputMouse)(LPDIRECTINPUT8 di, HWND hWnd, DWORD level, hdx_InputDevice * ppOut)
{
	IF_NULL(ppOut && di)
	{
		return E_INVALIDARG;
	}
	(*ppOut) = NULL;

	HRESULT hr;

	hdx_InputDevice mouse;
	IF_FAILED(hr = di->CreateDevice(
		GUID_SysMouse,
		&mouse,
		NULL))
	{
		return hr;
	}

	hr = ADD_FLAG(hr, mouse->SetDataFormat(&c_dfDIMouse2));
	hr = ADD_FLAG(hr, mouse->SetCooperativeLevel(hWnd, level));

	IF_FAILED(hr)
	{
		DEL_COM(mouse);
		return hr;
	}

	*ppOut = mouse;

	return S_OK;
}



REALIZE_FUNC(h::hdx_createInputKeyborad)(LPDIRECTINPUT8 di, HWND hWnd, DWORD level, hdx_InputDevice * ppOut)
{
	IF_NULL(ppOut && di)
	{
		return E_INVALIDARG;
	}
	(*ppOut) = NULL;

	HRESULT hr;

	hdx_InputDevice keyboard;
	IF_FAILED(hr = di->CreateDevice(
		GUID_SysKeyboard,
		&keyboard,
		NULL))
	{
		return hr;
	}

	hr = ADD_FLAG(hr, keyboard->SetDataFormat(&c_dfDIKeyboard));
	hr = ADD_FLAG(hr, keyboard->SetCooperativeLevel(hWnd, level));

	IF_FAILED(hr)
	{
		DEL_COM(keyboard);
		return hr;
	}

	*ppOut = keyboard;

	return S_OK;
}