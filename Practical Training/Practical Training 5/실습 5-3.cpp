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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 400, 0, 1200, 800, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
bool IsRectOverlap(const RECT* rect1, const RECT* rect2) {
	// 첫 번째 사각형의 오른쪽이 두 번째 사각형의 왼쪽보다 왼쪽에 있거나,
	// 첫 번째 사각형의 왼쪽이 두 번째 사각형의 오른쪽보다 오른쪽에 있는 경우 겹치지 않음
	if (rect1->right <= rect2->left || rect1->left >= rect2->right) {
		return false;
	}

	// 첫 번째 사각형의 아래쪽이 두 번째 사각형의 위쪽보다 위에 있거나,
	// 첫 번째 사각형의 위쪽이 두 번째 사각형의 아래쪽보다 아래에 있는 경우 겹치지 않음
	if (rect1->bottom <= rect2->top || rect1->top >= rect2->bottom) {
		return false;
	}

	// 위 조건에 해당하지 않는 경우 두 사각형은 겹침
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
void swap(Box* m1, Box* m2) {
	Box* a = m1;
	m1 = m2;
	m2 = a;
}
#define snake 10001
#define animation 10002
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, bitDC;
	static HBITMAP hBitmap, mybit[2];
	static BITMAP bmp;
	static RECT rt;
	static Box Lens,temp;
	static bool Drag = false, drawLens = true, ctrlv = false, FullScreen = false, Mirror = false, modv = false, modh = false, automove = false, animate = false, Left = false, Right = true, Up = false, Down = false, ChangeSize = true;
	static int Gapx, Gapy, LPercent{ 1000 }, mx, my, mx2, my2, mWidth[2], mHeight[2], PhotoSelect{ 0 }, Ppercent,count;
	// 메시지 처리하기
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt); //--- 윈도우 크기 얻어오기
		mybit[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		mybit[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		GetObject(mybit[0], sizeof(BITMAP), &bmp);
		mWidth[0] = bmp.bmWidth;
		mHeight[0] = bmp.bmHeight;
		GetObject(mybit[1], sizeof(BITMAP), &bmp);
		mWidth[1] = bmp.bmWidth;
		mHeight[1] = bmp.bmHeight;
		static HPEN redpen = CreatePen(PS_SOLID, 4, RGB(255, 50, 50));
		static HBRUSH Nullbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case WM_LBUTTONDOWN:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (drawLens)
		{
			Lens.rect.left = mx;
			Lens.rect.top = my;
			Lens.rect.right = mx;
			Lens.rect.bottom = my;
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_LBUTTONUP:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (drawLens)
		{
			Lens.rect.right = mx;
			Lens.rect.bottom = my;
			Lens.x = Lens.rect.right - Lens.rect.left;
			Lens.y = Lens.rect.bottom - Lens.rect.top;
			drawLens = false;
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InRectangle(Lens.rect, mx, my))
		{
			Gapx = mx - Lens.rect.left;
			Gapy = my - Lens.rect.top;
			Drag = true;
		}
		break;
	case WM_RBUTTONUP:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (Drag)
		{
			Drag = false;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_MOUSEMOVE:
		mx2 = LOWORD(lParam);
		my2 = HIWORD(lParam);
		if (Drag)
		{
			Lens.rect.left = mx2 - Gapx;
			Lens.rect.top = my2 - Gapy;
			Lens.rect.right = mx2 - Gapx + Lens.x;
			Lens.rect.bottom = my2 - Gapy + Lens.y;
			InvalidateRect(hWnd, NULL, false);
		}
		if (drawLens)
		{
			Lens.rect.right = mx2;
			Lens.rect.bottom = my2;
			InvalidateRect(hWnd, NULL, false);
		}
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		
		if (wParam == 'q' || wParam == 'Q')
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		else if (wParam == 'r' || wParam == 'R')
		{
			Lens.rect.left = 0;
			Lens.rect.top = 0;
			Lens.rect.right = 0;
			Lens.rect.bottom = 0;
			Lens.x = 0;
			Lens.y = 0;
			temp.rect.left = 0;
			temp.rect.top = 0;
			temp.rect.right = 0;
			temp.rect.bottom = 0;
			temp.x = 0;
			temp.y = 0;
			drawLens = true;
			modv = false;
			modh = false;
			Mirror = false;
			ctrlv = false;
			FullScreen = false;
			LPercent = 1000;
		}
		else if (wParam == 'e' || wParam == 'E')
		{
			if (LPercent > 100) {
				LPercent -= 100;
			}
		}
		else if (wParam == 's' || wParam == 'S')
		{
			LPercent += 100;
			if (Lens.x * LPercent > 1000 * rt.right || Lens.y * LPercent > 1000 * rt.bottom)
			{
				LPercent -= 100;
			}
		}
		else if (wParam == 'k')
		{
			if (Ppercent > 100) {
				Ppercent -= 100;
			}

		}
		else if (wParam == 'K')
		{
			Ppercent += 100;
			if (temp.x * Ppercent > 1000 * rt.right || temp.y * Ppercent > 1000 * rt.bottom)
			{
				Ppercent -= 100;
			}
		}
		else if (wParam == '0')
		{
			LPercent = 1000;
		}
		else if (wParam == '1')
		{
			PhotoSelect = 0;
		}
		else if (wParam == '2')
		{
			PhotoSelect = 1;
		}
		else if (wParam == 'b' || wParam == 'B')
		{
			if (animate)
			{
				animate = false;
				KillTimer(hWnd,animation);
			}
			else
			{
				animate = true;
				SetTimer(hWnd, animation, 50, NULL);
			}
		}
		else if (wParam == 'a' || wParam == 'A')
		{
			if (automove)
			{
				automove = false;
				KillTimer(hWnd, snake);
			}
			else
			{
				automove = true;
				SetTimer(hWnd, snake, 50, NULL);
			}
		}
		else if (wParam == 'h')
		{
			if (modh)
			{
				modh = false;
			}
			else
			{
				modh = true;
			}
		}
		else if (wParam == 'v')
		{
			if (modv)
			{
				modv = false;
			}
			else
			{
				modv = true;
			}
		}
		else if (wParam == 'i')
		{
			if (Mirror)
			{
				Mirror = false;
			}
			else
			{
				Mirror = true;
			}
		}
		else if (wParam == 'c'|| wParam == 'C')
		{
			temp = Lens;
			Ppercent = LPercent;
		}
		else if (wParam == 'f' || wParam == 'F')
		{
			if (ctrlv)
			{
				if (!FullScreen)
				{
					FullScreen = true;
				}
				else
				{
					FullScreen = false;
				}
			}
		}
		else if (wParam == 'p' || wParam == 'P')
		{
			ctrlv = true;
		}

		else if (wParam == 'm' || wParam == 'M')
		{
			Lens.x -= 10;
			Lens.y -= 10;
			Lens.rect.right -= 10;
			Lens.rect.bottom -= 10;
			if (Lens.rect.right < Lens.rect.left + 20 || Lens.rect.bottom < Lens.rect.top + 20)
			{
				Lens.x += 10;
				Lens.y += 10;
				Lens.rect.right += 10;
				Lens.rect.bottom += 10;
			}
		}
		else if (wParam == 'n' || wParam == 'N')
		{
			Lens.x += 10;
			Lens.y += 10;
			Lens.rect.right += 10;
			Lens.rect.bottom += 10;
			if (Lens.rect.right > rt.right || Lens.rect.bottom > rt.bottom)
			{
				Lens.x -= 10;
				Lens.y -= 10;
				Lens.rect.right -= 10;
				Lens.rect.bottom -= 10;
			}
		}
		else if (wParam == 'X')
		{
			Lens.x += 10;
			Lens.rect.right += 10;
			if (Lens.rect.right > rt.right)
			{
				Lens.x -= 10;
				Lens.rect.right -= 10;
			}
		}
		else if (wParam == 'x') {
			Lens.x -= 10;
			Lens.rect.right -= 10; 
			if (Lens.rect.right < Lens.rect.left + 20) {
				Lens.x += 10;
				Lens.rect.right += 10;
			}
		}
		else if (wParam == 'Y')
		{
			Lens.y += 10;
			Lens.rect.bottom += 10;
			if (Lens.rect.bottom > rt.bottom)
			{
				Lens.y -= 10;
				Lens.rect.bottom -= 10;
			}
		}
		else if (wParam == 'y') {
			Lens.y -= 10;
			Lens.rect.bottom -= 10;
			if (Lens.rect.bottom < Lens.rect.top + 20) {
				Lens.y += 10;
				Lens.rect.bottom += 10;
			}
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (wParam == VK_LEFT)
		{
			Lens.rect.left-=3;
			Lens.rect.right-=3;
		}
		else if (wParam == VK_RIGHT)
		{
			Lens.rect.left+=3;
			Lens.rect.right+=3;
		}
		else if (wParam == VK_UP)
		{
			Lens.rect.top-=3;
			Lens.rect.bottom-=3;
		}
		else if (wParam == VK_DOWN)
		{
			Lens.rect.top+=3;
			Lens.rect.bottom+=3;
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC); //--- 메모리 DC 만들기
		bitDC = CreateCompatibleDC(mDC);
		hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
		SelectObject(mDC, (HBITMAP)hBitmap); //--- 메모리 DC와 비트맵 연결하기
		//--- 모든 그리기를 메모리 DC에 한다.
		Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- 화면에 비어있기 때문에 화면 가득히 사각형을 그려 배경색으로 설정하기
		SelectObject(bitDC, mybit[PhotoSelect]);
		//전체화면 그리기
		StretchBlt(mDC, rt.left, rt.top, rt.right, rt.bottom, bitDC, 0, 0, mWidth[PhotoSelect], mHeight[PhotoSelect], SRCCOPY);
		//복사본 그리기
		if (ctrlv)
		{
			if (FullScreen)
			{
				if (Mirror)
				{
					SelectObject(mDC, redpen);
					SelectObject(mDC, Nullbrush);
					Rectangle(mDC, 0, 0, temp.x, temp.x* mHeight[PhotoSelect] / mWidth[PhotoSelect]);
					StretchBlt(mDC, 0, 0, temp.x, temp.x* mHeight[PhotoSelect] / mWidth[PhotoSelect], bitDC, 0, 0, mWidth[PhotoSelect], mHeight[PhotoSelect], NOTSRCCOPY);
				}
				else {
					SelectObject(mDC, redpen);
					SelectObject(mDC, Nullbrush);
					Rectangle(mDC, 0, 0, temp.x, temp.x* mHeight[PhotoSelect] / mWidth[PhotoSelect]);
					StretchBlt(mDC, 0, 0, temp.x, temp.x* mHeight[PhotoSelect] / mWidth[PhotoSelect], bitDC, 0, 0, mWidth[PhotoSelect], mHeight[PhotoSelect], SRCCOPY);
				}

			}
			else if (!FullScreen) {
				if (Mirror)
				{
					StretchBlt(mDC, 0, 0, temp.x, temp.y, bitDC, (temp.rect.left* mWidth[PhotoSelect] / rt.right) + (temp.x * mWidth[PhotoSelect] / rt.right) * ((1000 - Ppercent) / 2) / 1000, (temp.rect.top* mHeight[PhotoSelect] / rt.bottom) + (temp.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - Ppercent) / 2) / 1000, (temp.x* mWidth[PhotoSelect] / rt.right)* Ppercent / 1000, (temp.y* mHeight[PhotoSelect] / rt.bottom * Ppercent / 1000), NOTSRCCOPY);
					SelectObject(mDC, redpen);
					SelectObject(mDC, Nullbrush);
					Rectangle(mDC, 0, 0, temp.x, temp.y);
				}
				else
				{
					StretchBlt(mDC, 0, 0, temp.x, temp.y, bitDC, (temp.rect.left* mWidth[PhotoSelect] / rt.right) + (temp.x * mWidth[PhotoSelect] / rt.right) * ((1000 - Ppercent) / 2) / 1000, (temp.rect.top* mHeight[PhotoSelect] / rt.bottom) + (temp.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - Ppercent) / 2) / 1000, (temp.x* mWidth[PhotoSelect] / rt.right)* Ppercent / 1000, (temp.y* mHeight[PhotoSelect] / rt.bottom * Ppercent / 1000), SRCCOPY);
					SelectObject(mDC, redpen);
					SelectObject(mDC, Nullbrush);
					Rectangle(mDC, 0, 0, temp.x, temp.y);
				}

			}
		}
		//렌즈그리기
		if (Mirror)
		{
			if (modv && !modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000 + ((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000, -((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), NOTSRCCOPY);
			}
			else if (!modv && modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000, (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000 + (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), (Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000, -(Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), NOTSRCCOPY);
			}
			else if (modv && modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000 + ((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000 + (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), -((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), -(Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), NOTSRCCOPY);
			}
			else
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000, (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000, (Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000, (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), NOTSRCCOPY);
			}
		}
		else
		{
			if (modv&&!modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left* mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000 + ((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), (Lens.rect.top* mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000, -((Lens.x* mWidth[PhotoSelect] / rt.right)* LPercent / 1000), (Lens.y* mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), SRCCOPY);
			}
			else if (!modv&&modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left* mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000, (Lens.rect.top* mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000+ (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), (Lens.x* mWidth[PhotoSelect] / rt.right)* LPercent / 1000, -(Lens.y* mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), SRCCOPY);
			}
			else if (modv && modh)
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000 + ((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000+ (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), -((Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000), -(Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), SRCCOPY);
			}
			else
			{
				StretchBlt(mDC, Lens.rect.left, Lens.rect.top, Lens.x, Lens.y, bitDC, (Lens.rect.left * mWidth[PhotoSelect] / rt.right) + (Lens.x * mWidth[PhotoSelect] / rt.right) * ((1000 - LPercent) / 2) / 1000, (Lens.rect.top * mHeight[PhotoSelect] / rt.bottom) + (Lens.y * mWidth[PhotoSelect] / rt.bottom) * ((1000 - LPercent) / 2) / 1000, (Lens.x * mWidth[PhotoSelect] / rt.right) * LPercent / 1000, (Lens.y * mHeight[PhotoSelect] / rt.bottom * LPercent / 1000), SRCCOPY);
			}
		}
		//렌즈 틀 그리기
		SelectObject(mDC, redpen);
		SelectObject(mDC, Nullbrush);
		Rectangle(mDC, Lens.rect.left, Lens.rect.top, Lens.rect.right, Lens.rect.bottom);

		//--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
		DeleteDC(bitDC);
		DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
		DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case snake:
			if (Left)
			{
				Lens.rect.left-=5;
				Lens.rect.right-=5;
				if (Lens.rect.left < 0)
				{
					Lens.rect.left += 5;
					Lens.rect.right += 5;
					Lens.rect.top += 10;
					Lens.rect.bottom += 10;
					Left = false;
					Right = true;
					if (Lens.rect.bottom > rt.bottom)
					{
						Lens.rect.top -= 10;
						Lens.rect.bottom -= 10;
						Up = true;
						Left = false;
						Right = false;
					}
				}
			}
			else if (Right)
			{
				Lens.rect.left += 5;
				Lens.rect.right += 5;
				if (Lens.rect.right > rt.right)
				{
					Lens.rect.left -= 5;
					Lens.rect.right -= 5;
					Lens.rect.top += 10;
					Lens.rect.bottom += 10;
					Left = true;
					Right = false;
					if (Lens.rect.bottom > rt.bottom)
					{
						Lens.rect.top -= 10;
						Lens.rect.bottom -= 10;
					}
				}

			}
			else if (Up)
			{
				Lens.rect.bottom -= 5;
				Lens.rect.top -= 5;
				if (Lens.rect.top < 0)
				{
					Lens.rect.top += 5;
					Lens.rect.bottom += 5;
					Lens.rect.left += 10;
					Lens.rect.right += 10;
					Up = false;
					Down = true;
				}
			}
			else if (Down)
			{
				Lens.rect.bottom += 5;
				Lens.rect.top += 5;
				if (Lens.rect.bottom > rt.bottom)
				{
					Lens.rect.top -= 5;
					Lens.rect.bottom -= 5;
					Lens.rect.left += 10;
					Lens.rect.right += 10;
					Up = true;
					Down = false;
				}
			}
			break;
		case animation:
			count++;
			if (ChangeSize)
			{
				Lens.x += 6;
				Lens.y += 6;
				Lens.rect.top -= 3;
				Lens.rect.bottom += 3;
				Lens.rect.left -= 3;
				Lens.rect.right += 3;
				if (count == 30)
				{
					ChangeSize = false;
					count = 0;
				}
			}
			else
			{
				Lens.x -= 6;
				Lens.y -= 6;
				Lens.rect.top += 3;
				Lens.rect.bottom -= 3;
				Lens.rect.left += 3;
				Lens.rect.right -= 3;
				if (count == 30)
				{
					ChangeSize = true;
					count = 0;
				}
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, false); //--- 화면에 다시그리기를 할 때 기존의 내용을 삭제하지 않는다.
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}