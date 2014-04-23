// flight.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "flight.h"
#include <objidl.h>
#include <gdiplus.h> 
using namespace Gdiplus;
#pragma comment( lib, "gdiplus.lib" ) 

#include <vector>
#include "particle.h"
using std::vector;

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// Gdiplus �� 
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Pen *g_pen; // �� ��ü.
Brush *g_brush; // �귯�� ��ü.
Brush *g_bulletBrush;
Bitmap *g_bmp;
Image *g_bg;
Rect g_hero(0, 0, 50, 50);
vector<Rect> g_bullets(2);
int g_front = 0;
int g_back = 0;
cParticleManager g_particleMng;

enum GAME_MODE
{
	SHOOTING,
	RACING,
};
GAME_MODE g_mode = RACING;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// Gdiplus �ʱ�ȭ.
void InitGdiPlus(HWND hWnd)
{
	//Start Gdiplus 
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_brush = new SolidBrush(Color::White);
	g_bulletBrush = new SolidBrush(Color::Red);

	RECT cr;
	GetClientRect(hWnd, &cr);
	g_bmp = new Bitmap(cr.right-cr.left, cr.bottom-cr.top);

	g_bg = Image::FromFile(L"bg.jpg");
}

// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_bmp;
	delete g_bg;
	delete g_bulletBrush;
	delete g_pen;
	delete g_brush;
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
	LoadString(hInstance, IDC_FLIGHT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLIGHT));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ReleaseGdiPlus();
	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLIGHT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_FLIGHT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
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

   InitGdiPlus(hWnd);

   SetTimer(hWnd, 0, 1, NULL);

   RECT cr;
   GetClientRect(hWnd, &cr);
   const int x = (cr.right-cr.left) / 2;
   const int y = cr.bottom - g_hero.Height - 1;
   g_hero.Offset(x, y);

   if (SHOOTING == g_mode)
   {
	   for (int i=0; i < (int)g_bullets.size(); ++i)
	   {
		   int xx = rand() % cr.right;
		   g_bullets[ i] = Rect(xx, 0, 10, 30);
	   }
   }
   else
   {
	   g_bullets[ 0] = Rect(cr.right/2 - 100, 0, 10, 30);
	   g_bullets[ 1] = Rect(cr.right/2 + 100, 0, 10, 30);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
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

	case WM_ERASEBKGND:
		return 0;

	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);

			Graphics *graph = Graphics::FromImage(g_bmp);
			//graph->FillRectangle(g_brush, wndSize);
			graph->DrawImage(g_bg, wndSize);

			for (int i=0; i < (int)g_bullets.size(); ++i)
				graph->FillRectangle(g_bulletBrush, g_bullets[ i]);

			graph->FillRectangle(g_brush, g_hero);
			g_graphics->DrawImage(g_bmp, wndSize);

			g_particleMng.Render(hdc);
			EndPaint(hWnd, &ps);
		}
		break;

	//case WM_KEYDOWN:
	//	switch (wParam)
	//	{
	//	//case VK_LEFT: g_Hero.Offset(-10, 0); break;
	//	//case VK_RIGHT: g_Hero.Offset(10, 0); break;
	//	case VK_UP:
	//	case VK_DOWN:
	//		break;
	//	}
	//	::InvalidateRect(hWnd, NULL, TRUE);
	//	break;

	case WM_TIMER:
		{
			RECT cr;
			GetClientRect(hWnd, &cr);

			if (SHOOTING == g_mode)
			{
				for (int i=0; i < (int)g_bullets.size(); ++i)
				{
					g_bullets[ i].Offset(0,10);
					if (g_bullets[ i].Y > cr.bottom)
					{
						g_bullets[ i].Offset(-g_bullets[ i].X,-g_bullets[ i].Y);
						g_bullets[ i].Offset(rand()%cr.right, 0);
					}
				}
			}
			else if (RACING == g_mode)
			{
				for (int i=0; i < (int)g_bullets.size(); ++i)
				{
					g_bullets[ i].Offset(0,10);
				}

				if (g_bullets[ g_back].Y > 20)
				{
					Rect r1 = g_bullets[ g_back];
					Rect r2 = g_bullets[ g_back+1];
					const int offset = (rand() % 40) - 20;
					r1.Offset(offset, -r1.Y);
					r2.Offset(offset, -r2.Y);

					if (cr.bottom < g_bullets[ g_front].Y)
					{
						g_bullets[ g_front] = r1;
						g_bullets[ g_front+1] = r2;

						g_back = g_front;

						g_front+=2;
						g_front %= g_bullets.size();
					}
					else
					{
						g_bullets.push_back(r1);
						g_bullets.push_back(r2);
						g_back += 2;
					}
				}
			}

			bool isRefresh = false;
			if ((GetKeyState(VK_LEFT) & 0x100) != 0)
			{
				isRefresh = true;
				g_hero.Offset(-20, 0);
			}
			if ((GetKeyState(VK_RIGHT) & 0x100) != 0)
			{
				isRefresh = true;
				g_hero.Offset(20, 0);
			}

			for (int i=0; i < (int)g_bullets.size(); ++i)
			{
				if (g_bullets[ i].IntersectsWith(g_hero))
				{// collision
					POINT pos = {g_bullets[ i].X, g_bullets[i].Y};
					g_particleMng.AddParticle( new cParticle(pos) );
				}
			}

			static int oldT = GetTickCount();
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			oldT = curT;

			g_particleMng.Move(elapseT);
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
