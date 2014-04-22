// bounceBall.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "bounceBall.h"
#include <objidl.h>
#include <gdiplus.h> 
using namespace Gdiplus;
#pragma comment( lib, "gdiplus.lib" ) 

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Gdiplus �� 
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Pen *g_pen; // �� ��ü.


// 2���� ���� ����ü.
struct sVector
{
	float x,y;
};
sVector g_ballPos = {300,300}; // �� ��ġ
sVector g_ballVel; // �� �ӷ�

// Gdiplus �ʱ�ȭ.
void InitGdiPlus(HWND hWnd)
{
	//Start Gdiplus 
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
}

// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_graphics;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOUNCEBALL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNCEBALL));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ReleaseGdiPlus(); // Gdiplus ����.
	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNCEBALL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOUNCEBALL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   InitGdiPlus(hWnd); // Gdiplus ����.

   // �ʱ� Ball �ӵ� �ʱ�ȭ.
   g_ballVel.x = 200;
   g_ballVel.y = 300;

   // Ÿ�̸� ����.
   SetTimer(hWnd, 0, 1, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// ����ġ g_ballPos �� ���� ����Ѵ�.
		g_graphics->DrawEllipse(g_pen, (int)g_ballPos.x, (int)g_ballPos.y, 100, 100);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		{
			// Ÿ�̸� �̺�Ʈ.
			// 1�ʿ� 1000�� ȣ���ϰ� ������, �����δ� 100������ ȣ�� �ȴ�.

			// WM_TIMER �̺�Ʈ ȣ�� ������ ����Ѵ�.  (�ʴ���)
			static int oldT = GetTickCount();
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			const float eT = (float)elapseT / 1000.f;
			oldT = curT;

			// �� ���� �ܰ��� �ε�ġ�� ƨ�ܳ����� �Ѵ�.
			RECT cr;
			GetClientRect(hWnd, &cr); // �� ���� ũ�⸦ ��´�.

			// left, right �浹 �˻�, �浹 �ߴٸ� x�� �ӵ� ����.
			if (((g_ballVel.x < 0) && (cr.left > (int)g_ballPos.x)) || 
				((g_ballVel.x > 0) && (cr.right < (int)g_ballPos.x)))
				g_ballVel.x = -g_ballVel.x;

			// top, bottom �浹 �˻�, �浹 �ߴٸ� y�� �ӵ� ����.
			if (((g_ballVel.y < 0) && (cr.top > (int)g_ballPos.y)) || 
				((g_ballVel.y  > 0) && (cr.bottom < (int)g_ballPos.y)))
				g_ballVel.y = -g_ballVel.y;

			// �ӵ� * ����ð� = �̵� �Ÿ�.
			// ����ġ g_ballPos �� �̵� �Ÿ� ��ŭ ���� �߰��Ѵ�.
			g_ballPos.x += g_ballVel.x * eT;
			g_ballPos.y += g_ballVel.y * eT;

			// ȭ�� ��������
			::InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

