#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <cmath>
#include "resource.h"
HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 5-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 850, 0, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
bool IsRectOverlap(const RECT* rect1, const RECT* rect2) {
	// ù ��° �簢���� �������� �� ��° �簢���� ���ʺ��� ���ʿ� �ְų�,
	// ù ��° �簢���� ������ �� ��° �簢���� �����ʺ��� �����ʿ� �ִ� ��� ��ġ�� ����
	if (rect1->right <= rect2->left || rect1->left >= rect2->right) {
		return false;
	}

	// ù ��° �簢���� �Ʒ����� �� ��° �簢���� ���ʺ��� ���� �ְų�,
	// ù ��° �簢���� ������ �� ��° �簢���� �Ʒ��ʺ��� �Ʒ��� �ִ� ��� ��ġ�� ����
	if (rect1->bottom <= rect2->top || rect1->top >= rect2->bottom) {
		return false;
	}

	// �� ���ǿ� �ش����� �ʴ� ��� �� �簢���� ��ħ
	return true;
}
BOOL InRectangle(RECT rect, int mx, int my) {
	if (mx <= rect.right && mx >= rect.left && my <= rect.bottom && my >= rect.top)
	{
		return true;
	}
	return false;
}
float LengthPts(int x1, int y1, int x2, int y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
struct Box {
	RECT rect;
	int direct;
	int status;
	int x;
	int y;
	double theta;
	int shape;
	HBRUSH color;
	int size;
	bool Select;
	int x2;
	int y2;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, bitDC;
	static HBITMAP hBitmap, mybit;
	static BITMAP bmp;
	static RECT rt;
	static Box ScreenBox[2][2];
	static bool base = false, Screenmod = false,DevideScreen = false;
	static int count = 0, Center_x, Center_y, mx, my, mWidth, mHeight;
	// �޽��� ó���ϱ�
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt); //--- ������ ũ�� ������
		mybit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(mybit, sizeof(BITMAP), &bmp);
		mWidth = bmp.bmWidth;
		mHeight = bmp.bmHeight;
		static HPEN redpen = CreatePen(PS_SOLID, 4, RGB(255,50,50));
		static HBRUSH Nullbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		Center_x = rt.right;
		Center_y = rt.bottom;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ScreenBox[i][j].rect.left = rt.left;
				ScreenBox[i][j].rect.right = rt.right;
				ScreenBox[i][j].rect.top = rt.top;
				ScreenBox[i][j].rect.bottom = rt.bottom;
				ScreenBox[i][j].Select = false;
				ScreenBox[i][j].x = 0;
			}
		}
		ScreenBox[0][0].rect.left = 0;
		ScreenBox[0][0].rect.top = 0;
		ScreenBox[0][0].rect.right = rt.right;
		ScreenBox[0][0].rect.bottom = rt.bottom;
		ScreenBox[0][0].Select = true;
		break;
	case WM_LBUTTONDOWN:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j ++)
			{
				if (InRectangle(ScreenBox[i][j].rect,mx,my))
				{
					for (int a = 0; a < 2; a++)
					{
						for (int b = 0; b < 2; b++)
						{
							ScreenBox[a][b].Select = false;
						}
					}
					ScreenBox[i][j].Select = true;
				}
			}
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == 'a' || wParam == 'A' && !base)
		{
			base = true;
		}
		else if (wParam == 'r' || wParam == 'R')
		{
			if (!Screenmod)
			{
				Screenmod = true;
			}
			else
			{
				Screenmod = false;
			}
		}
		else if (wParam == '+')
		{

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (ScreenBox[i][j].Select)
					{
						if (i == 0 && j == 0) {
							Center_x += 4;
							Center_y += 3;
						}
						else if (i == 0 && j == 1) {
							Center_x -= 4;
							Center_y += 3;
						}
						else if (i == 1 && j == 0) {
							Center_x += 4;
							Center_y -= 3;
						}
						else if (i == 1 && j == 1) {
							Center_x -= 4;
							Center_y -= 3;
						}
						ScreenBox[0][0].rect.right = Center_x;
						ScreenBox[0][0].rect.bottom = Center_y;
						ScreenBox[0][1].rect.left = Center_x;
						ScreenBox[0][1].rect.bottom = Center_y;
						ScreenBox[1][0].rect.right = Center_x;
						ScreenBox[1][0].rect.top = Center_y;
						ScreenBox[1][1].rect.left = Center_x;
						ScreenBox[1][1].rect.top = Center_y;
					}
				}

			}
		}
		else if (wParam == '-')
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (ScreenBox[i][j].Select)
					{
						if (i == 0 && j == 0) {
							Center_x -= 4;
							Center_y -= 3;
						}
						else if (i == 0 && j == 1) {
							Center_x += 4;
							Center_y -= 3;
						}
						else if (i == 1 && j == 0) {
							Center_x -= 4;
							Center_y += 3;
						}
						else if (i == 1 && j == 1) {
							Center_x += 4;
							Center_y += 3;
						}
						ScreenBox[0][0].rect.right = Center_x;
						ScreenBox[0][0].rect.bottom = Center_y;
						ScreenBox[0][1].rect.left = Center_x;
						ScreenBox[0][1].rect.bottom = Center_y;
						ScreenBox[1][0].rect.right = Center_x;
						ScreenBox[1][0].rect.top = Center_y;
						ScreenBox[1][1].rect.left = Center_x;
						ScreenBox[1][1].rect.top = Center_y;
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (wParam == VK_LEFT)
		{
			DevideScreen = true;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (ScreenBox[i][j].Select)
					{
						ScreenBox[i][j].x--;
					}
				}
			}
		}
		else if (wParam == VK_RIGHT)
		{
			DevideScreen = true;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (ScreenBox[i][j].Select)
					{
						ScreenBox[i][j].x++;
						if (ScreenBox[i][j].x == ScreenBox[i][j].rect.right- ScreenBox[i][j].rect.left)
						{
							ScreenBox[i][j].x = 0;
						}
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT:
		
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC); //--- �޸� DC �����
		bitDC = CreateCompatibleDC(mDC);
		hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- �޸� DC�� ������ ��Ʈ�� �����
		SelectObject(mDC, (HBITMAP)hBitmap); //--- �޸� DC�� ��Ʈ�� �����ϱ�
		//--- ��� �׸��⸦ �޸� DC�� �Ѵ�.
		Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- ȭ�鿡 ����ֱ� ������ ȭ�� ������ �簢���� �׷� �������� �����ϱ�

		if (!base)
		{
			SelectObject(bitDC, mybit);
			BitBlt(mDC, 0, 0, 300, 168, bitDC, 0, 0, SRCCOPY);
		}
		else if(base)
		{
			SelectObject(bitDC, mybit);
			if (Screenmod)
			{
				StretchBlt(mDC, 0, 0, 800, 600, bitDC, 0, 0, 300, 168, NOTSRCCOPY);
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{

						if (ScreenBox[i][j].x >= 0)
						{
							//����
							StretchBlt(mDC, ScreenBox[i][j].rect.left, ScreenBox[i][j].rect.top, ScreenBox[i][j].x, ScreenBox[i][j].rect.bottom - ScreenBox[i][j].rect.top, bitDC, 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 0, 300 - 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 168, SRCCOPY);
							//������
							StretchBlt(mDC, ScreenBox[i][j].rect.left + ScreenBox[i][j].x, ScreenBox[i][j].rect.top, ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x, ScreenBox[i][j].rect.bottom - ScreenBox[i][j].rect.top, bitDC, 0, 0, 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 168, SRCCOPY);
						}
						else
						{
							//����
							StretchBlt(mDC, ScreenBox[i][j].rect.left, ScreenBox[i][j].rect.top, ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left + ScreenBox[i][j].x, ScreenBox[i][j].rect.bottom - ScreenBox[i][j].rect.top, bitDC, 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 0, 300 - 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 168, SRCCOPY);
							//������
							StretchBlt(mDC, ScreenBox[i][j].rect.left + ScreenBox[i][j].x, ScreenBox[i][j].rect.top, - ScreenBox[i][j].x, ScreenBox[i][j].rect.bottom - ScreenBox[i][j].rect.top, bitDC, 0, 0, 300 * (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left - ScreenBox[i][j].x) / (ScreenBox[i][j].rect.right - ScreenBox[i][j].rect.left), 168, SRCCOPY);
						}
					}
				}
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						if (ScreenBox[i][j].Select)
						{
							SelectObject(mDC, redpen);
							SelectObject(mDC, Nullbrush);
							Rectangle(mDC, ScreenBox[i][j].rect.left, ScreenBox[i][j].rect.top, ScreenBox[i][j].rect.right, ScreenBox[i][j].rect.bottom);
						}
					}
				}

			};
		}
		

		//--- �������� �޸� DC�� ������ ȭ�� DC�� �����Ѵ�.
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
		DeleteDC(bitDC);
		DeleteDC(mDC); //--- ������ �޸� DC ����
		DeleteObject(hBitmap); //--- ������ ��Ʈ�� ����
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false); //--- ȭ�鿡 �ٽñ׸��⸦ �� �� ������ ������ �������� �ʴ´�.
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}