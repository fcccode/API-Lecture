// WindowPenBrush.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "WindowPenBrush.h"
#include <math.h>
#include <windowsx.h>
#include <vector>

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HPEN g_hPen;
HBRUSH g_hBrush;
bool g_lbtnDown = false;
POINT g_downPos;
POINT g_movePos;

struct sLine {
	int state; // 0=down, 1=move, 2=up
	POINT pos;
};
std::vector<sLine> g_Lines; // ���׸���


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_WINDOWPENBRUSH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	//--------------------------------------------------------------------------------------------------
	// �� ����.
	g_hPen = CreatePen(
		PS_SOLID,	// �� Ÿ�� ex) PS_SOLID, PS_DASH, PS_DOT
		1,				// �� ��
		RGB(255,0,0) ); // ����.


	//--------------------------------------------------------------------------------------------------
	// �귯�� ����

	// ������ ä��� �귯��
	//g_hBrush = CreateSolidBrush(RGB(0,0,255)); 

	// �ټ����� ä��� �귯��
	g_hBrush = CreateHatchBrush(
		HS_BDIAGONAL, // ex) HS_CROSS, HS_BDIAGONAL ...
		RGB(0,0,255)); 


	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWPENBRUSH));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	DeleteObject(g_hPen);

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWPENBRUSH));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOWPENBRUSH);
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
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_TEST1:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			SetTextColor(hdc, RGB(255,0,0));	// �ؽ�Ʈ ���� ����
			//SetBkColor(hdc, RGB(0,255,0)); // �ؽ�Ʈ ��� ���� ����

			TextOutA( hdc, 10, 10, "hello World", 11);	// x,y ��ġ�� �ؽ�Ʈ ���
			RECT r;
			SetRect(&r, 100, 100, 200, 200);	
			DrawTextA( hdc, "hello World", 11, &r, DT_CENTER); //�簢�� ��ġ��  �ؽ�Ʈ ���

			HPEN oldPen = (HPEN)SelectObject(hdc, g_hPen); // Pen ����

			// ���� �߱�.
			LineTo(hdc, 100, 100);
			LineTo(hdc, 500, 100);
			MoveToEx(hdc, 500, 200, NULL);
			LineTo(hdc, 600, 300);

			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, g_hBrush); // �귯�� ����.

			Rectangle(hdc, 300, 300, 400, 400);	// �ڽ� ���
			Ellipse(hdc, 400, 300, 500, 400);	// Ÿ���� ���

			// sin �
			MoveToEx(hdc, 0, 300, NULL);
			for (float i=0; i < 10; i+=0.1f)
			{
				float y = sin(i) * 100;
				LineTo(hdc, (int)(i*100.f), (int)(y + 300.f));
			}

			if (g_lbtnDown)
			{
				//MoveToEx(hdc, g_downPos.x, g_downPos.y, NULL);
				//LineTo(hdc, g_movePos.x, g_movePos.y);
			}

			for (int i=0; i < (int)g_Lines.size(); ++i)
			{
				if (g_Lines[ i].state == 0)
					MoveToEx(hdc, g_Lines[ i].pos.x, g_Lines[ i].pos.y, NULL);
				else if (g_Lines[ i].state == 1)
					LineTo(hdc, g_Lines[ i].pos.x, g_Lines[ i].pos.y);
			}
			
			SelectObject(hdc, oldPen); // ���� Pen���� ����.
			SelectObject(hdc, oldBrush); // ���� Brush�� ����
			EndPaint(hWnd, &ps);
		}
		break;

	// ���콺 ���� ��ư �ٿ� �̺�Ʈ
	case WM_LBUTTONDOWN:
		{
			g_lbtnDown = true;
			g_downPos.x = GET_X_LPARAM(lParam); 
			g_downPos.y = GET_Y_LPARAM(lParam); 

			sLine line;
			line.state = 0;
			line.pos = g_downPos;
			g_Lines.push_back(line);
		}
		break;

	// ���콺 ���� ��ư �� �̺�Ʈ
	case WM_LBUTTONUP:
		{
			g_lbtnDown = false;

			sLine line;
			line.state = 2;
			g_Lines.push_back(line);
		}
		break;

	// ���콺 �̵� �̺�Ʈ 
	case WM_MOUSEMOVE:
		if (g_lbtnDown)
		{
			POINT pos;
			pos.x = GET_X_LPARAM(lParam); 
			pos.y = GET_Y_LPARAM(lParam); 
			g_movePos = pos;
			InvalidateRect(hWnd, NULL, TRUE);

			sLine line;
			line.state = 1;
			line.pos = pos;
			g_Lines.push_back(line);
		}
		break;

	// Ű���� �ٿ�
	case WM_KEYDOWN:
		break;
	
	case WM_ERASEBKGND:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
