#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <cmath>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 3-4";
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 950, 200, 500, 480, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
struct figure {
	int x;
	int y;
	int movecount;
	HBRUSH color;
	bool rightdown;
	bool leftdown;
};

#define mapsize 11
#define p1move 30
#define p2move 50
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC, mDC;
	static HBITMAP hBitmap;
	static RECT rt;
	static bool PLAY = false, finish1 = false, finish2 = false;
	int mx, my;
	static int count{ 0 }, count2{ 0 };
	TCHAR Clear1[200];
	TCHAR Clear2[200];
	TCHAR Clear3[200];
	static figure player1{ 400,280,0 }, player2{400,320,0};
	static figure PrevP1, PrevP2;
	switch (iMsg)
	{
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rt); //--- ������ ũ�� ������
		player1.color = CreateSolidBrush(RGB(255, 0, 0));
		player2.color = CreateSolidBrush(RGB(0, 0, 255));
		player1.rightdown = false;
		player1.leftdown = false;
		player2.rightdown = false;
		player2.leftdown = false;
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
			if (wParam == 'p'|| wParam == 'P')
			{
				PLAY = true;
				InvalidateRect(hWnd, NULL, false);
			}
			else if (wParam == 's' || wParam == 'S')
			{
				player1.x = 400, player1.y = 280, player2.x = 400,player2.y = 320;
				finish1 = false, finish2 = false;
				player1.rightdown = false;
				player1.leftdown = false;
				player2.rightdown = false;
				player2.leftdown = false;
			}
			else if (wParam == '1')
			{
				player1.movecount = rand() % 5 + 1;
				count = player1.movecount;
				PrevP1.x = player1.x;
				PrevP1.y = player1.y;
				SetTimer(hWnd, p1move, 500, NULL);
			}
			else if (wParam == '2')
			{
				player2.movecount = rand() % 5 + 1;
				count2 = player2.movecount;
				PrevP2.x = player2.x;
				PrevP2.y = player2.y;
				SetTimer(hWnd, p2move, 500, NULL);

			}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC); //--- �޸� DC �����
		hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- �޸� DC�� ������ ��Ʈ�� �����
		SelectObject(mDC, (HBITMAP)hBitmap); //--- �޸� DC�� ��Ʈ�� �����ϱ�
		Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- ȭ�鿡 ����ֱ� ������ ȭ�� ������ �簢���� �׷� �������� �����ϱ�
		//--- ��� �׸��⸦ �޸� DC�� �Ѵ�.
		if (PLAY) {
			//����
			for (int i = 0; i < 6; i++)
			{
				RoundRect(mDC, 60 * i + 30, 30, 60 * i + 60, 60, 10, 10);
			}
			//�º�
			for (int i = 0; i < 6; i++)
			{
				RoundRect(mDC, 30, 60 * i + 30, 60, 60 * i + 60, 10, 10);
			}
			//�غ�
			for (int i = 0; i < 6; i++)
			{
				RoundRect(mDC, 60 * i + 30, 330, 60 * i + 60, 360, 10, 10);
			}
			//�캯
			for (int i = 0; i < 6; i++)
			{
				RoundRect(mDC, 330, 60 * i + 30, 360, 60 * i + 60, 10, 10);
			}
			//������ �밢
			for (int i = 0; i < 7; i++)
			{
				RoundRect(mDC, 50 * i + 30, 50 * i + 30, 50 * i + 60, 50 * i + 60, 10, 10);
			}
			//�»��� �밢
			for (int i = 0; i < 7; i++)
			{
				RoundRect(mDC, 50 * (6 - i) + 30, 50 * i + 30, 50 * (6 - i) + 60, 50 * i + 60, 10, 10);
			}
		}
		//�÷��̾� ������
		RoundRect(mDC, 390, 270, 440, 360, 10, 10);
		//�÷��̾� �׸���

		SelectObject(mDC, player1.color);
		RoundRect(mDC, player1.x, player1.y, player1.x+30, player1.y+30, 10, 10);
		SelectObject(mDC, player2.color);
		RoundRect(mDC, player2.x, player2.y, player2.x+30, player2.y+30, 10, 10);
		switch (player1.movecount) {
		case 1:
			wsprintf(Clear1, L"�÷��̾� 1 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 370, Clear1, lstrlen(Clear1));
			break;
		case 2:
			wsprintf(Clear1, L"�÷��̾� 1 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 370, Clear1, lstrlen(Clear1));
			break;
		case 3:
			wsprintf(Clear1, L"�÷��̾� 1 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 370, Clear1, lstrlen(Clear1));
			break;
		case 4:
			wsprintf(Clear1, L"�÷��̾� 1 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 370, Clear1, lstrlen(Clear1));
			break;
		case 5:
			wsprintf(Clear1, L"�÷��̾� 1 : ���Դϴ� �ټ�ĭ �����Դϴ�.");
			TextOut(mDC, 50, 370, Clear1, lstrlen(Clear1));
			break;
		}
		switch (player2.movecount) {
		case 1:
			wsprintf(Clear1, L"�÷��̾� 2 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 390, Clear1, lstrlen(Clear1));
			break;
		case 2:
			wsprintf(Clear1, L"�÷��̾� 2 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 390, Clear1, lstrlen(Clear1));
			break;
		case 3:
			wsprintf(Clear1, L"�÷��̾� 2 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 390, Clear1, lstrlen(Clear1));
			break;
		case 4:
			wsprintf(Clear1, L"�÷��̾� 2 : ���Դϴ� ��ĭ �����Դϴ�.");
			TextOut(mDC, 50, 390, Clear1, lstrlen(Clear1));
			break;
		case 5:
			wsprintf(Clear1, L"�÷��̾� 2 : ���Դϴ� �ټ�ĭ �����Դϴ�.");
			TextOut(mDC, 50, 390, Clear1, lstrlen(Clear1));
			break;
		}
		if (finish1)
		{
			wsprintf(Clear1, L"���ϵ帳�ϴ�, �÷��̾� 1�� �¸��Դϴ�!");
			TextOut(mDC, 50, 420, Clear1, lstrlen(Clear1));
		}
		if (finish2)
		{
			wsprintf(Clear1, L"���ϵ帳�ϴ�, �÷��̾� 2�� �¸��Դϴ�!");
			TextOut(mDC, 50, 420, Clear1, lstrlen(Clear1));
		}
		//--- �������� �޸� DC�� ������ ȭ�� DC�� �����Ѵ�.
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
		DeleteDC(mDC); //--- ������ �޸� DC ����
		DeleteObject(hBitmap); //--- ������ ��Ʈ�� ����
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case p1move:
			
			if (count > 0)
			{
				/* Ư�� ������ �ִ� ���*/ 
				// �ǴϽ� ������ ���
				if (player1.x == 330 && player1.y == 330)
				{
					player1.x = 400;
					player1.y = 281;
					finish1 = true;
					player1.leftdown = false;
					player1.rightdown = false;
				}
				//���� ������ �ִ� ���
				else if (player1.x == 400 && player1.y == 280)
				{
					player1.x = 330;
					player1.y = 330;

				}

				// �����ϴ��� ���
				else if (player1.x == 30 && player1.y == 330)
				{
					player1.leftdown = false;
				}
				//���� ��ܿ��� ������ ���
				else if (PrevP1.x == 30 && PrevP1.y == 30)
				{
					player1.rightdown = true;
				}
				//������ܿ��� ������ ���
				else if (PrevP1.x == 330 && PrevP1.y == 30)
				{
					player1.leftdown = true;
				}
				//���߾ӿ��� ������ ���
				else if (PrevP1.x == 180 && PrevP1.y == 180)
				{
					player1.rightdown = true;
					player1.leftdown = false;
				}
				//������ �밢�� �ִ� ���
				if (player1.rightdown&&!finish1)
				{
					player1.x += 50;
					player1.y += 50;
				}
				//������ �밢�� �ִ� ��� (���������� �߾����� ��ġ ���� �ľ����� �ʿ�)
				else if (player1.leftdown && !finish1)
				{
					player1.x -= 50;
					player1.y += 50;
				}
				//�캯�� �ִ� ��� 
				else if (player1.x == 330 && player1.y > 70)
				{
					player1.y -= 60;
				
				}
				//������ �ִ� ��� 
				else if (player1.y == 30&& player1.x > 40)
				{
					player1.x -= 60;
				}
				//�º��� �ִ� ���
				else if (player1.x == 30 && player1.y < 320)
				{
					player1.y += 60;
				}
				//�غ��� �ִ� ���
				else if (player1.y == 330 && player1.x < 320)
				{
					player1.x += 60;
				}
				count--;
			}

			InvalidateRect(hWnd, NULL, false);
			break;
		case p2move:

			if (count2 > 0)
			{
				/* Ư�� ������ �ִ� ���*/

				// �ǴϽ� ������ ���
				if (player2.x == 330 && player2.y == 330)
				{
					player2.x = 400;
					player2.y = 321;
					finish2 = true;
					player2.leftdown = false;
					player2.rightdown = false;
				}

				//���� ������ �ִ� ���
				else if (player2.x == 400 && player2.y == 320)
				{
					player2.x = 330;
					player2.y = 330;

				}

				// �����ϴ��� ���
				else if (player2.x == 30 && player2.y == 330)
				{
					player2.leftdown = false;
				}
				//���� ��ܿ��� ������ ���
				else if (PrevP2.x == 30 && PrevP2.y == 30)
				{
					player2.rightdown = true;
				}
				//������ܿ��� ������ ���
				else if (PrevP2.x == 330 && PrevP2.y == 30)
				{
					player2.leftdown = true;
				}
				//���߾ӿ��� ������ ���
				else if (PrevP2.x == 180 && PrevP2.y == 180)
				{
					player2.rightdown = true;
					player2.leftdown = false;
				}
				//������ �밢�� �ִ� ���
				if (player2.rightdown && !finish2)
				{
					player2.x += 50;
					player2.y += 50;
				}
				//������ �밢�� �ִ� ��� (���������� �߾����� ��ġ ���� �ľ����� �ʿ�)
				else if (player2.leftdown && !finish2)
				{
					player2.x -= 50;
					player2.y += 50;
				}
				//�캯�� �ִ� ��� 
				else if (player2.x == 330 && player2.y > 70)
				{
					player2.y -= 60;

				}
				//������ �ִ� ��� 
				else if (player2.y == 30 && player2.x > 40)
				{
					player2.x -= 60;
				}
				//�º��� �ִ� ���
				else if (player2.x == 30 && player2.y < 320)
				{
					player2.y += 60;
				}
				//�غ��� �ִ� ���
				else if (player2.y == 330 && player2.x < 320)
				{
					player2.x += 60;
				}
				count2--;
			}

			InvalidateRect(hWnd, NULL, false);
			break;
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}