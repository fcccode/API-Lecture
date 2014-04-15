// windowBase.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "windowBase.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
// TCHAR
// �����ڵ� �� ���� wchar_t Ÿ���� ����Ų��.
// ��Ƽ����Ʈ �ڵ� �� ���� char Ÿ���� ����Ų��.


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,	// �ν��Ͻ� �ڵ�, Ư�� �Լ��� ȣ���� �� �ʿ��ϴ�.
                     HINSTANCE hPrevInstance, // ���� ����
                     LPTSTR    lpCmdLine, // Ŀ�ǵ� ���� ��Ʈ��
                     int       nCmdShow // ������ â�� ȭ�鿡 ��Ÿ���� ����. ex) SW_SHOWNORMAL, SW_MAXIMIZE
					 )
{
	UNREFERENCED_PARAMETER(hPrevInstance); // �������� �ʴ� �Ķ���Ͱ� warning  �Ǵ� ���� �����ϱ� ����.
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOWBASE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg; // ������ �޼��� ����ü
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWBASE));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

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
	// ������ Ŭ���� ���
	// ������ �޼��� ���ν���, ������, �޴�, Ŭ���� �̸��� ����Ѵ�.

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;		// ������ �޼��� ���ν��� ����
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWBASE));	// ������ ����
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);	// Ŀ�� ����
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOWBASE); // �޴� ����
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ������ ����

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

   // â�� �����մϴ�.
   hWnd = CreateWindow(
	   szWindowClass, 
	   szTitle,		// â �̸� ����
	   WS_OVERLAPPEDWINDOW, // â Ÿ�� ����
      CW_USEDEFAULT,	// â ��ġ X
	  0, // â ��ġ Y
	  CW_USEDEFAULT,  // â ��
	  0, // â ����
	  NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); // â�� ȭ�鿡 ���̰� �Ѵ�.
   UpdateWindow(hWnd); // â ȭ�� ����
   
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
	PAINTSTRUCT ps; // ����Ʈ ����ü
	HDC hdc; // Device Context �ڵ�

	// WM_~  WindowMessage �� ����.
	switch (message)
	{
	case WM_COMMAND:		// �޴��� �����ϸ� �߻��Ǵ� �̺�Ʈ
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	// ȭ�� ���� �޼���
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);	// Device Context �ڵ� ����.
			
			TextOutA( hdc, 10, 10, "hello World", 11);
			RECT r;
			SetRect(&r, 100, 100, 200, 200);
			DrawTextA( hdc, "hello World", 11, &r, DT_CENTER);

			EndPaint(hWnd, &ps);		// ȭ�� ���� ����
		}
		break;

	// ������ ���� �޼���
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// �� �ۿ� ������ �޼����� DefWindowProc() �Լ����� ó���Ѵ�.
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
