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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
void Shuffle_Box(Box (*box)[5], int size) {
	Box temp;
	int x, y;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int x = rand() % size;
			int y = rand() % size;
			temp = box[i][j];
			box[i][j] = box[x][y];
			box[x][y] = temp;
		}
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, bitDC;
	static HBITMAP hBitmap, mybit[2];
	static BITMAP bmp;
	static RECT rt;
	static Box ScreenBox[5][5],temp,temp2,MAP[5][5];
	static bool FullScreen = false, Mirror = false, GameStart = false, Drag = false, gameover = false, modv = false, modh = false;
	static int Gapx[5], Gapy[5], mx, my, mx2, my2, mWidth[2], mHeight[2], PhotoSelect{0}, Mapsize, ScreenSelectX{0}, ScreenSelectY{0}, samecount;
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
		mHeight [1] = bmp.bmHeight;
		static HFONT GMOV = CreateFont(144, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Impact");
		static HPEN redpen = CreatePen(PS_SOLID, 4, RGB(255, 50, 50));
		static HBRUSH Nullbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				ScreenBox[i][j].rect.left = 0;
				ScreenBox[i][j].rect.right = 0;
				ScreenBox[i][j].rect.top = 0;
				ScreenBox[i][j].rect.bottom = 0;
				ScreenBox[i][j].status = 1;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < Mapsize; i++)
		{
			for (int j = 0; j < Mapsize; j++)
			{
				if (InRectangle(ScreenBox[i][j].rect, mx, my) && !ScreenBox[i][j].Select && GameStart)
				{
					ScreenSelectX = j;
					ScreenSelectY = i;

					if (modv)
					{
						for (int k = 0; k < Mapsize; k++)
						{
							Gapx[k] = mx - ScreenBox[i][k].rect.left;
							Gapy[k] = my - ScreenBox[i][k].rect.top;
						}
					}
					else if(modh)
					{
						for (int k = 0; k < Mapsize; k++)
						{
							Gapx[k] = mx - ScreenBox[k][j].rect.left;
							Gapy[k] = my - ScreenBox[k][j].rect.top;
						}
					}
					else
					{
						Gapx[0] = mx - ScreenBox[i][j].rect.left;
						Gapy[0] = my - ScreenBox[i][j].rect.top;
					}
					Drag = true;
				}
			}
		}
		
		
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_MOUSEMOVE:
		mx2 = LOWORD(lParam);
		my2 = HIWORD(lParam);
		if (Drag && GameStart && !modv && !modh)
		{
			temp2.rect = ScreenBox[ScreenSelectY][ScreenSelectX].rect;
			ScreenBox[ScreenSelectY][ScreenSelectX].rect.left = mx2 - Gapx[0];
			ScreenBox[ScreenSelectY][ScreenSelectX].rect.top = my2 - Gapy[0];
			ScreenBox[ScreenSelectY][ScreenSelectX].rect.right = mx2 - Gapx[0] + ScreenBox[ScreenSelectY][ScreenSelectX].x;
			ScreenBox[ScreenSelectY][ScreenSelectX].rect.bottom = my2 - Gapy[0] + ScreenBox[ScreenSelectY][ScreenSelectX].y;
			
		}
		else if (Drag && GameStart && modv && !modh)
		{
			for (int i = 0; i < Mapsize; i++)
			{
				temp2.rect = ScreenBox[ScreenSelectY][i].rect;
				ScreenBox[ScreenSelectY][i].rect.left = mx2 - Gapx[i];
				ScreenBox[ScreenSelectY][i].rect.top = my2 - Gapy[i];
				ScreenBox[ScreenSelectY][i].rect.right = mx2 - Gapx[i] + ScreenBox[ScreenSelectY][i].x;
				ScreenBox[ScreenSelectY][i].rect.bottom = my2 - Gapy[i] + ScreenBox[ScreenSelectY][i].y;
			}
		}
		else if (Drag && GameStart && !modv && modh)
		{
			for (int i = 0; i < Mapsize; i++)
			{
				ScreenBox[i][ScreenSelectX].rect.left = mx2 - Gapx[i];
				ScreenBox[i][ScreenSelectX].rect.top = my2 - Gapy[i];
				ScreenBox[i][ScreenSelectX].rect.right = mx2 - Gapx[i] + ScreenBox[i][ScreenSelectX].x;
				ScreenBox[i][ScreenSelectX].rect.bottom = my2 - Gapy[i] + ScreenBox[i][ScreenSelectX].y;
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		hDC = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		
		for (int i = 0; i < Mapsize; i++)
		{
			for (int j = 0; j < Mapsize; j++)
			{
				if (InRectangle(MAP[i][j].rect, mx, my)&&Drag && GameStart)
				{
					if (!modv&&!modh)
					{
						if (ScreenBox[i][j].Select)
						{
							ScreenBox[i][j].direct = ScreenBox[ScreenSelectY][ScreenSelectX].direct;
							ScreenBox[i][j].size = ScreenBox[ScreenSelectY][ScreenSelectX].size;
							int temp = ScreenBox[i][j].status;
							ScreenBox[i][j].status = ScreenBox[ScreenSelectY][ScreenSelectX].status;
							ScreenBox[ScreenSelectY][ScreenSelectX].status = temp;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.left = MAP[ScreenSelectY][ScreenSelectX].rect.left;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.right = MAP[ScreenSelectY][ScreenSelectX].rect.right;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.bottom = MAP[ScreenSelectY][ScreenSelectX].rect.bottom;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.top = MAP[ScreenSelectY][ScreenSelectX].rect.top;
							ScreenBox[ScreenSelectY][ScreenSelectX].Select = true;
							ScreenBox[i][j].Select = false;
						}
						else
						{
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.left = MAP[ScreenSelectY][ScreenSelectX].rect.left;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.right = MAP[ScreenSelectY][ScreenSelectX].rect.right;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.bottom = MAP[ScreenSelectY][ScreenSelectX].rect.bottom;
							ScreenBox[ScreenSelectY][ScreenSelectX].rect.top = MAP[ScreenSelectY][ScreenSelectX].rect.top;
						}
					}
					else if (modv)
					{
						int temp;
						//값 변경
						for (int k = 0; k < Mapsize; k++)
						{
							temp = ScreenBox[i][k].direct;
							ScreenBox[i][k].direct = ScreenBox[ScreenSelectY][k].direct;
							ScreenBox[ScreenSelectY][k].direct = temp;
							temp = ScreenBox[i][k].size;
							ScreenBox[i][k].size = ScreenBox[ScreenSelectY][k].size;
							ScreenBox[ScreenSelectY][k].size = temp;
							temp = ScreenBox[i][k].status;
							ScreenBox[i][k].status = ScreenBox[ScreenSelectY][k].status;
							ScreenBox[ScreenSelectY][k].status = temp;
						}
						//위치 원위치
						for (int i = 0; i < Mapsize; i++)
						{
							for (int j = 0; j < Mapsize; j++)
							{
								ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
								ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
								ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
								ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
								ScreenBox[i][j].x = rt.right / Mapsize;
								ScreenBox[i][j].y = rt.bottom / Mapsize;
							}
						}
					}
					else if (modh)
					{
						int temp;
						//값 변경
						for (int k = 0; k < Mapsize; k++)
						{
							temp = ScreenBox[k][j].direct;
							ScreenBox[k][j].direct = ScreenBox[k][ScreenSelectX].direct;
							ScreenBox[k][ScreenSelectX].direct = temp;
							temp = ScreenBox[k][j].size;
							ScreenBox[k][j].size = ScreenBox[k][ScreenSelectX].size;
							ScreenBox[k][ScreenSelectX].size = temp;
							temp = ScreenBox[k][j].status;
							ScreenBox[k][j].status = ScreenBox[k][ScreenSelectX].status;
							ScreenBox[k][ScreenSelectX].status = temp;
						}
						//위치 원위치
						for (int i = 0; i < Mapsize; i++)
						{
							for (int j = 0; j < Mapsize; j++)
							{
								ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
								ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
								ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
								ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
								ScreenBox[i][j].x = rt.right / Mapsize;
								ScreenBox[i][j].y = rt.bottom / Mapsize;
							}
						}
					}
				}
			}
		}		
		for (int i = 0; i < Mapsize; i++)
		{
			for (int j = 0; j < Mapsize; j++)
			{
				if (ScreenBox[i][j].status == MAP[i][j].status)
				{
					samecount++;
				}
			}
		}
		if (samecount == Mapsize * Mapsize) {
			gameover = true;
			modv = false;
			modh = false;
			GameStart = false;
		}
		samecount = 0;

		ScreenSelectX = 5;
		ScreenSelectY = 5;
		Drag = false;
		
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == 's' || wParam == 'S')
		{
			GameStart = true;
		}
		else if (wParam == 'q' || wParam == 'Q')
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		if (!GameStart)
		{
			if (wParam == 'v' || wParam == 'V' && !GameStart && !modv && !modh)
			{
				modh = false;
				modv = true;
				GameStart = true;
				gameover = false;
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						ScreenBox[i][j].status = i * Mapsize + j + 1;
						ScreenBox[i][j].Select = false;
						ScreenBox[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
						ScreenBox[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					}
				}
				Box temp;
				int x, y;
				for (int i = 0; i < Mapsize; i++)
				{
					int x = rand() % Mapsize;
					for (int j = 0; j < Mapsize; j++)
					{
						temp = ScreenBox[i][j];
						ScreenBox[i][j] = ScreenBox[x][j];
						ScreenBox[x][j] = temp;
					}
				}
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
						ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
						ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
						ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
						ScreenBox[i][j].x = rt.right / Mapsize;
						ScreenBox[i][j].y = rt.bottom / Mapsize;
						MAP[i][j].rect.left = rt.right / Mapsize * j;
						MAP[i][j].rect.right = rt.right / Mapsize * (j + 1);
						MAP[i][j].rect.top = rt.bottom / Mapsize * i;
						MAP[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
						MAP[i][j].x = rt.right / Mapsize;
						MAP[i][j].y = rt.bottom / Mapsize;
						MAP[i][j].status = i * Mapsize + j + 1;
						MAP[i][j].Select = false;
						MAP[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
						MAP[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					}
				}
			}
			else if (wParam == 'h' || wParam == 'H' && !GameStart && !modv && !modh)
			{
				modh = true;
				modv = false;
				GameStart = true;
				gameover = false;
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						ScreenBox[i][j].status = i * Mapsize + j + 1;
						ScreenBox[i][j].Select = false;
						ScreenBox[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
						ScreenBox[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					}
				}
				Box temp;
				int x, y;
				for (int i = 0; i < Mapsize; i++)
				{
					int x = rand() % Mapsize;
					for (int j = 0; j < Mapsize; j++)
					{
						temp = ScreenBox[j][i];
						ScreenBox[j][i] = ScreenBox[j][x];
						ScreenBox[j][x] = temp;
					}
				}
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
						ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
						ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
						ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
						ScreenBox[i][j].x = rt.right / Mapsize;
						ScreenBox[i][j].y = rt.bottom / Mapsize;
						MAP[i][j].rect.left = rt.right / Mapsize * j;
						MAP[i][j].rect.right = rt.right / Mapsize * (j + 1);
						MAP[i][j].rect.top = rt.bottom / Mapsize * i;
						MAP[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
						MAP[i][j].x = rt.right / Mapsize;
						MAP[i][j].y = rt.bottom / Mapsize;
						MAP[i][j].status = i * Mapsize + j + 1;
						MAP[i][j].Select = false;
						MAP[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
						MAP[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					}
				}
			}
		}

		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (wParam == 'f'|| wParam == 'F')
		{
			FullScreen = true;
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYUP:
		hDC = GetDC(hWnd);
		if (wParam == 'f'|| wParam == 'F')
		{
			FullScreen = false;
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
		if (GameStart)
		{
			if (!Mirror)
			{
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						if (!ScreenBox[i][j].Select)
						{
							StretchBlt(mDC, ScreenBox[i][j].rect.left, ScreenBox[i][j].rect.top, ScreenBox[i][j].x, ScreenBox[i][j].y, bitDC, ScreenBox[i][j].size, ScreenBox[i][j].direct, mWidth[PhotoSelect] / Mapsize, mHeight[PhotoSelect] / Mapsize, SRCCOPY);
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < Mapsize; i++)
				{
					for (int j = 0; j < Mapsize; j++)
					{
						if (!ScreenBox[i][j].Select)
						{
							StretchBlt(mDC, ScreenBox[i][j].rect.left, ScreenBox[i][j].rect.top, ScreenBox[i][j].x, ScreenBox[i][j].y, bitDC, ScreenBox[i][j].size, ScreenBox[i][j].direct, mWidth[PhotoSelect] / Mapsize, mHeight[PhotoSelect] / Mapsize, NOTSRCCOPY);
						}
					}
				}
			}
		}
		if (gameover)
		{
			SelectObject(mDC, GMOV);
			SetBkMode(mDC, TRANSPARENT);
			DrawText(mDC, L"Game Clear", lstrlen(L"Game Clear"), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		if (FullScreen)
		{
			StretchBlt(mDC, rt.left, rt.top, rt.right, rt.bottom, bitDC, 0, 0, mWidth[PhotoSelect], mHeight[PhotoSelect], SRCCOPY);
		}
		//--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
		DeleteDC(bitDC);
		DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
		DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false); //--- 화면에 다시그리기를 할 때 기존의 내용을 삭제하지 않는다.
		break;
	case WM_COMMAND:
		hDC = GetDC(hWnd);
		switch (LOWORD(wParam)) {
		case ID_40001:
			PhotoSelect = 0;
			break;
		case ID_40002:
			PhotoSelect = 1;
			break;
		case ID_40003:
			Mapsize = 3;
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].status = i * Mapsize + j+1;
					ScreenBox[i][j].Select = false;
					ScreenBox[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					ScreenBox[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
				}
			}
			Shuffle_Box(ScreenBox, Mapsize);
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
					ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
					ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
					ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					ScreenBox[i][j].x = rt.right / Mapsize;
					ScreenBox[i][j].y = rt.bottom / Mapsize;
					MAP[i][j].rect.left = rt.right / Mapsize * j;
					MAP[i][j].rect.right = rt.right / Mapsize * (j + 1);
					MAP[i][j].rect.top = rt.bottom / Mapsize * i;
					MAP[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					MAP[i][j].x = rt.right / Mapsize;
					MAP[i][j].y = rt.bottom / Mapsize;
					MAP[i][j].status = i * Mapsize + j + 1;
					MAP[i][j].Select = false;
					MAP[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					MAP[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
				}
			}
			ScreenBox[rand() % Mapsize][rand() % Mapsize].Select = true;
			break;
		case ID_40004:
			Mapsize = 4;
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].status = i * Mapsize + j + 1;
					ScreenBox[i][j].Select = false;
					ScreenBox[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					ScreenBox[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					MAP[i][j].rect.left = rt.right / Mapsize * j;
					MAP[i][j].rect.right = rt.right / Mapsize * (j + 1);
					MAP[i][j].rect.top = rt.bottom / Mapsize * i;
					MAP[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					MAP[i][j].x = rt.right / Mapsize;
					MAP[i][j].y = rt.bottom / Mapsize;
					MAP[i][j].status = i * Mapsize + j + 1;
					MAP[i][j].Select = false;
					MAP[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					MAP[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
				}
			}
			Shuffle_Box(ScreenBox, Mapsize);
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
					ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
					ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
					ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					ScreenBox[i][j].x = rt.right / Mapsize;
					ScreenBox[i][j].y = rt.bottom / Mapsize;
				}
			}
			ScreenBox[rand() % Mapsize][rand() % Mapsize].Select = true;
			break;
		case ID_40005:
			Mapsize = 5;
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].status = i * Mapsize + j + 1;
					ScreenBox[i][j].Select = false;
					ScreenBox[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					ScreenBox[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
					MAP[i][j].rect.left = rt.right / Mapsize * j;
					MAP[i][j].rect.right = rt.right / Mapsize * (j + 1);
					MAP[i][j].rect.top = rt.bottom / Mapsize * i;
					MAP[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					MAP[i][j].x = rt.right / Mapsize;
					MAP[i][j].y = rt.bottom / Mapsize;
					MAP[i][j].status = i * Mapsize + j + 1;
					MAP[i][j].Select = false;
					MAP[i][j].size = mWidth[PhotoSelect] / Mapsize * j;
					MAP[i][j].direct = mHeight[PhotoSelect] / Mapsize * i;
				}
			}
			Shuffle_Box(ScreenBox, Mapsize);
			for (int i = 0; i < Mapsize; i++)
			{
				for (int j = 0; j < Mapsize; j++)
				{
					ScreenBox[i][j].rect.left = rt.right / Mapsize * j;
					ScreenBox[i][j].rect.right = rt.right / Mapsize * (j + 1);
					ScreenBox[i][j].rect.top = rt.bottom / Mapsize * i;
					ScreenBox[i][j].rect.bottom = rt.bottom / Mapsize * (i + 1);
					ScreenBox[i][j].x = rt.right / Mapsize;
					ScreenBox[i][j].y = rt.bottom / Mapsize;
				}
			}
			ScreenBox[rand() % Mapsize][rand() % Mapsize].Select = true;
			break;
		case ID_40006:
			GameStart = true;
			gameover = false;
			break;
		case ID_40007:
			if (FullScreen)
			{
				FullScreen = false;
			}
			else
			{
				FullScreen = true;
			}
			break;
		case ID_40008:
			if (Mirror)
			{
				Mirror = false;
			}
			else
			{
				Mirror = true;
			}
			break;
		case ID_40009:
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}