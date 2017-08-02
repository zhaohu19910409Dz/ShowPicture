#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "IS_String.h"
#include "IS.h"
#include "IS_RenderDevice11.h"
#include "IS_VertexBuffer.h"
#include "IS_Vector2.h"
#include "IS_Vector3.h"
#include "IS_Texture2D.h"
#include "IS_VertexShader.h"
#include "IS_PixelShader.h"

struct VertexType
{
	VertexType()
	{
		position = IS::Vector3();
		texture = IS::Vector2();
	}

	VertexType(IS::Vector3 p, IS::Vector2 t)
	{
		position = p;
		texture = t;
	}

	IS::Vector3 position;
	IS::Vector2 texture;
};

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*HDC hdc;
	PAINTSTRUCT ps;*/

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcl;
	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hInstance = hInstance;
	wcl.lpszClassName = _T("ISWndClass");
	wcl.lpfnWndProc = MainWndProc;
	wcl.style = CS_DBLCLKS;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = NULL;
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	if (!RegisterClassEx(&wcl))
	{
		MessageBox(0, _T("注册窗口类失败!"), 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT r = { 0, 0, 800, 600 };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	int iWidth = r.right - r.left;
	int iHeight = r.bottom - r.top;
	int iLeft = (GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2;
	int iTop = (GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2;

	HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW, _T("ISWndClass"), _T("IS"),
		WS_OVERLAPPEDWINDOW, iLeft, iTop, iWidth, iHeight,
		NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		MessageBox(0, _T("创建窗口失败!"), 0, 0);
		return false;
	}

	IS::RenderDevice* device = new IS::RenderDevice11();
	if (device->Initialize(hwnd, 800, 600) != IS::EC_None)
	{
		MessageBox(0, _T("初始化设备失败!"), 0, 0);
		return 0;
	}

	VertexType* vertexData = new VertexType[4];
	memset(vertexData, 0, sizeof(VertexType) * 4);

	vertexData[0] = VertexType(IS::Vector3(-0.5f, -0.5f, 0), IS::Vector2(0, 1));
	vertexData[1] = VertexType(IS::Vector3(-0.5f, 0.5f, 0), IS::Vector2(0, 0));
	vertexData[2] = VertexType(IS::Vector3(0.5f, 0.5f, 0), IS::Vector2(1, 0));
	vertexData[3] = VertexType(IS::Vector3(0.5f, -0.5f, 0), IS::Vector2(1, 1));

	IS::Buffer* vertex = device->CreateBuffer(IS::BT_Vertex, vertexData, sizeof(VertexType), 4);

	unsigned int* indexData = new unsigned int[6];
	memset(indexData, 0, sizeof(unsigned int) * 6);

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 0;
	indexData[4] = 2;
	indexData[5] = 3;

	IS::Buffer* index = device->CreateBuffer(IS::BT_Index, indexData, sizeof(unsigned int), 6);

	IS::Texture2D* diffuseTexture = device->CreateTexture(IS::TT_Texture2D, IS::String(_T("../Assets/Textures/test.png")));

	IS::VertexShader* diffuseVertexShader = (IS::VertexShader*)device->CreateShader(IS::SS_Vertex, \
		IS::String(_T("../Assets/Shaders/Diffuse.vs")));

	IS::PixelShader* diffusePixelShader = (IS::PixelShader*)device->CreateShader(IS::SS_Pixel, \
		IS::String(_T("../Assets/Shaders/Diffuse.ps")));

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	bool done = false;
	MSG msg = { 0 };

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			device->Render(vertex, index, diffuseTexture, diffuseVertexShader, diffusePixelShader);
		}

	}

	return 0;
}