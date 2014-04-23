// bounceBall.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "bounceBall.h"
#include <objidl.h>
#include <gdiplus.h> 
#include <math.h>
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
Brush *g_brush; // �귯�� ��ü.
Bitmap *g_bmp;

enum {
	WIDTH = 50,
	MAX_BALL = 10,
};

// 2���� ���� ����ü.
struct sVector
{
	float x, y;
	sVector() {}
	sVector(float _x, float _y) : x(_x), y(_y) {}
	sVector& operator=(const sVector &rhs) // �Ҵ� ������.
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}
		return *this;
	}
	sVector operator-(const sVector &rhs) const // - ������.
	{
		sVector v;
		v.x = x - rhs.x;
		v.y = y - rhs.y;
		return v;
	}
	sVector operator+(const sVector &rhs) const // + ������
	{
		sVector v;
		v.x = x + rhs.x;
		v.y = y + rhs.y;
		return v;
	}
	sVector operator*(const sVector &rhs) const // * ������
	{
		sVector v;
		v.x = x * rhs.x;
		v.y = y * rhs.y;
		return v;
	}
	template<class T>
	sVector operator*(const T &f) const // ��Į�� �� ������.
	{
		sVector v;
		v.x = x * f;
		v.y = y * f;
		return v;
	}
	float length() { return (float)sqrt(x*x + y*y); } // ���� ���� ����
	void normalize() // ���� ����ȭ 
	{
		const float L = length();
		x /= L;
		y /= L;
	}
	void interpol( const sVector &from, const sVector &to, float f ) // ���� ����.
	{
		*this = from*(1.f-f) + to*f;
		normalize();
	}
};


//sVector g_ballPos(300,300); // �� ��ġ
//sVector g_ballVel; // �� �ӷ�

struct sBall
{
	sVector pos;
	sVector vel;
};

sBall g_ball[ MAX_BALL]; // �� �迭.


// Gdiplus �ʱ�ȭ.
void InitGdiPlus(HWND hWnd)
{
	//Start Gdiplus 
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_brush = new SolidBrush(Color::White);

	RECT cr;
	GetClientRect(hWnd, &cr);
	g_bmp = new Bitmap(cr.right-cr.left, cr.bottom-cr.top);
}

// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_bmp;
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
   //g_ballVel.x = 200;
   //g_ballVel.y = 300;

   // Ÿ�̸� ����.
   SetTimer(hWnd, 0, 1, NULL);

   // �� �迭 �ʱ�ȭ.
   for (int i=0; i < MAX_BALL; ++i)
   {
	   g_ball[ i].pos = sVector((float)i*WIDTH, (float)i*WIDTH);
	   g_ball[ i].vel = sVector(200,300);
   }

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

	case WM_ERASEBKGND:
		return 0;

	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			RECT rect;
			GetClientRect(hWnd, &rect);
			RectF destRect(REAL(rect.left), REAL(rect.top), REAL(rect.right-rect.left), REAL(rect.bottom-rect.top));

			Graphics *graph = Graphics::FromImage(g_bmp);
			graph->FillRectangle(g_brush, rect.left, rect.top, rect.right, rect.bottom);

			// ����ġ g_ballPos �� ���� ����Ѵ�.
			// �� ���.
			for (int i=0; i < MAX_BALL; ++i)
				graph->DrawEllipse(g_pen, (int)g_ball[ i].pos.x, (int)g_ball[ i].pos.y, WIDTH, WIDTH);				

			g_graphics->DrawImage(g_bmp, destRect);

			EndPaint(hWnd, &ps);
		}
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

			for (int i=0; i < MAX_BALL; ++i)
			{
				// left, right �浹 �˻�, �浹 �ߴٸ� x�� �ӷ� ����.
				if (((g_ball[ i].vel.x < 0) && (cr.left > (int)g_ball[ i].pos.x)) || 
					((g_ball[ i].vel.x > 0) && (cr.right < (int)g_ball[ i].pos.x+WIDTH)))
					g_ball[ i].vel.x = -g_ball[ i].vel.x;

				// top, bottom �浹 �˻�, �浹 �ߴٸ� y�� �ӷ� ����.
				if (((g_ball[ i].vel.y < 0) && (cr.top > (int)g_ball[ i].pos.y)) || 
					((g_ball[ i].vel.y  > 0) && (cr.bottom < (int)g_ball[ i].pos.y+WIDTH)))
					g_ball[ i].vel.y = -g_ball[ i].vel.y;

				// �ӵ� * ����ð� = �̵� �Ÿ�.
				// ����ġ g_ballPos �� �̵� �Ÿ� ��ŭ ���� �߰��Ѵ�.
				g_ball[ i].pos.x += g_ball[ i].vel.x * eT;
				g_ball[ i].pos.y += g_ball[ i].vel.y * eT;

				for (int k=0; k < MAX_BALL; ++k)
				{
					if (i == k)
						continue;

					// �� ���� �Ÿ��� �� ���� ������ ���� ���� �۴ٸ� �浹 �� ���̴�. 
					sVector dist = g_ball[ i].pos - g_ball[ k].pos;
					if (WIDTH >= dist.length())
					{// collision
						sVector dir = g_ball[ i].pos - g_ball[ k].pos;
						dir.normalize(); // �� ���� ���� ���� ���,  �������ͷ� �ʱ�ȭ.

						// ��ġ���� (�浹 ��ġ)
						g_ball[ i].pos = dir * (WIDTH) + g_ball[ k].pos; // ���� ��ġ�� �ʰ� ��ġ ����
						g_ball[ i].vel = dir * g_ball[ i].vel.length(); // �浹 �ݻ簢 ������Ʈ.
					}
				}
			}

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

