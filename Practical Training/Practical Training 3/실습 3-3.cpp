#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <cmath>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 3-3";
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 850, 0, 1000, 1000, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
BOOL InRectangle(RECT rect,int mx,int my) {
	if (mx <= rect.right && mx >= rect.left&& my <= rect.bottom && my >= rect. top)
	{
		return true;
	}
	return false;
}
struct figure {
	int x;
	int x2;
	int y;
	int y2;
	HBRUSH color;
};
#define SignalLight 50
#define Amount 8
#define Traffic 100
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC, mDC;
	static HBITMAP hBitmap;
	static RECT rt;
	static RECT Left_Right = { 20, 120, 90, 190 };
	static RECT Up_Down{ 700, 120, 770, 190 };
	TCHAR Clear1[200];
	TCHAR Clear2[200];
	TCHAR Clear3[200];
	int mx, my;
	static bool yellow = false, red = false, green = false;
	static figure Car[Amount];
	static figure Person;
	static int carsize{ 20 }, CarSpeed{ 25 }, timecount1{ 0 }, timecount2{ 0 }, timecount3{ 0 };
	// �޽��� ó���ϱ�
	switch (uMsg)
	{
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rt); //--- ������ ũ�� ������
		static HPEN carline = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		static HBRUSH YellowLight = CreateSolidBrush(RGB(250, 200, 0));
		static HBRUSH RedLight = CreateSolidBrush(RGB(255, 0, 0));
		static HBRUSH GreenLight = CreateSolidBrush(RGB(0,250, 100));
		static HBRUSH BlackLight = CreateSolidBrush(RGB(0, 0, 0));
		static HBRUSH ClickBoard = CreateSolidBrush(RGB(200, 100, 100));
		static HBRUSH player = CreateSolidBrush(RGB(100, 100, 100));
		Person.x = rt.right / 3  - 30;
		Person.y = rt.bottom / 3 - 30;
		for (int i = 0; i < Amount; i++)
		{
			Car[i].color = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		Car[0].x = 150, Car[0].y = rt.bottom / 2 + 3 * carsize;
		Car[1].x = 20, Car[1].y = rt.bottom / 2 + 3 * carsize;
		Car[2].x = rt.right - 230, Car[2].x2 = rt.right, Car[2].y = rt.bottom / 3 + 5 * carsize / 2;
		Car[3].x = rt.right - 100, Car[3].x2 = rt.right, Car[3].y = rt.bottom / 3 + 5 * carsize / 2;
		Car[4].x = rt.right / 3 + 5 * carsize / 2, Car[4].y = 150;
		Car[5].x = rt.right / 3 + 5 * carsize / 2, Car[5].y = 20;
		Car[6].x = rt.right / 2 + 5 * carsize / 2, Car[6].y = rt.bottom - 221, Car[6].y2 = rt.bottom;
		Car[7].x = rt.right / 2 + 5 * carsize / 2, Car[7].y = rt.bottom - 80, Car[7].y2 = rt.bottom;
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == 'a'||wParam =='A')
		{
			green = true;
			red = false;
			yellow = false;
			SetTimer(hWnd, SignalLight, 1000, NULL);
			SetTimer(hWnd, Traffic, CarSpeed, NULL);
		}
		else if (wParam == '+')
		{
			if (CarSpeed >1)
			{
				CarSpeed--;
				SetTimer(hWnd, Traffic, CarSpeed, NULL);
			}
		}
		else if (wParam == '-')
		{
			CarSpeed++;
			SetTimer(hWnd, Traffic, CarSpeed, NULL);
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InRectangle(Left_Right,mx,my))
		{
			green = true;
			yellow = false;
			red = false;
			timecount1 = 0;
			timecount2 = 0;
			timecount3 = 0;
		}
		if (InRectangle(Up_Down, mx, my))
		{
			green = false;
			yellow = false;
			red = true;
			timecount1 = 0;
			timecount2 = 0;
			timecount3 = 0;
		}
		break;
	case WM_PAINT:
		
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC); //--- �޸� DC �����
		hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- �޸� DC�� ������ ��Ʈ�� �����
		SelectObject(mDC, (HBITMAP)hBitmap); //--- �޸� DC�� ��Ʈ�� �����ϱ�
		//--- ��� �׸��⸦ �޸� DC�� �Ѵ�.
		Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- ȭ�鿡 ����ֱ� ������ ȭ�� ������ �簢���� �׷� �������� �����ϱ�

		// �¿� ��ȣ�� �׸���
		SelectObject(mDC, BlackLight);
		RoundRect(mDC, 20, 20, 240, 100,20,20);
		SelectObject(mDC, BlackLight);
		if (green == true && yellow == false)
		{
			SelectObject(mDC, GreenLight);
		}
		Ellipse(mDC, 30, 30, 90, 90);
		SelectObject(mDC, BlackLight);
		if (yellow)
		{
			SelectObject(mDC, YellowLight);
		}
		Ellipse(mDC, 100, 30, 160, 90);
		SelectObject(mDC, BlackLight);
		if (red == true && yellow == false)
		{
			SelectObject(mDC, RedLight);
		}
		Ellipse(mDC, 170, 30, 230, 90);
		//���� ��ȣ�� �׸���
		SelectObject(mDC, BlackLight);
		RoundRect(mDC, 700, 20, 930, 100,20,20);
		SelectObject(mDC, BlackLight);
		if (green == true && yellow == false)
		{
			SelectObject(mDC, RedLight);
		}
		Ellipse(mDC, 860, 30, 920, 90);
		SelectObject(mDC, BlackLight);
		if (yellow)
		{
			SelectObject(mDC, YellowLight);
		}
		Ellipse(mDC, 790, 30, 850, 90);
		SelectObject(mDC, BlackLight);
		if (red == true && yellow == false)
		{
			SelectObject(mDC, GreenLight);
		}
		Ellipse(mDC, 720, 30, 780, 90);
		SelectObject(mDC, ClickBoard);
		RoundRect(mDC, 700, 120, 770, 190,20,20);
		SelectObject(mDC, ClickBoard);
		RoundRect(mDC, 20, 120, 90, 190,20,20);
		wsprintf(Clear1, L"�� / ��");
		TextOut(mDC, 33, 146, Clear1, lstrlen(Clear1));
		wsprintf(Clear1, L"�� / �� ");
		TextOut(mDC, 713, 146, Clear1, lstrlen(Clear1));
		// ���� Ʋ �׸���
		wsprintf(Clear1, L"timecount1 : %d", timecount1);
		TextOut(mDC, 100, 120, Clear1, lstrlen(Clear1));
		wsprintf(Clear2, L"timecount2 : %d", timecount2);
		TextOut(mDC, 100, 140, Clear2, lstrlen(Clear2));
		wsprintf(Clear3, L"timecount3 : %d", timecount3);
		TextOut(mDC, 100, 160, Clear3, lstrlen(Clear3));
		MoveToEx(mDC, (rt.right) / 3, 0, NULL);
		LineTo(mDC, (rt.right) / 3, (rt.bottom) / 3);
		MoveToEx(mDC, (rt.right) * 2 / 3, 0, NULL);
		LineTo(mDC, (rt.right) * 2 / 3, (rt.bottom) / 3);
		MoveToEx(mDC, 0, (rt.bottom) / 3, NULL);
		LineTo(mDC, (rt.right)/3, (rt.bottom) / 3);
		MoveToEx(mDC, 0, (rt.bottom) * 2 / 3, NULL);
		LineTo(mDC, (rt.right)/3, (rt.bottom) * 2 / 3);
		MoveToEx(mDC, (rt.right)/3, (rt.bottom) * 2 / 3, NULL);
		LineTo(mDC, (rt.right)/3, rt.bottom);
		MoveToEx(mDC, (rt.right)*2/3, (rt.bottom) * 2 / 3, NULL);
		LineTo(mDC, (rt.right)*2/3, rt.bottom);
		MoveToEx(mDC, rt.right, (rt.bottom) / 3, NULL);
		LineTo(mDC, (rt.right)*2/3, (rt.bottom) / 3);
		MoveToEx(mDC, (rt.right)*2/3, (rt.bottom) * 2 / 3, NULL);
		LineTo(mDC, rt.right, (rt.bottom) * 2 / 3);
		//�߾Ӽ� �׸���
		MoveToEx(mDC, 0, (rt.bottom)/2, NULL);
		LineTo(mDC, rt.right, (rt.bottom) / 2);
		MoveToEx(mDC, (rt.right) / 2, 0, NULL);
		LineTo(mDC, (rt.right) / 2, rt.bottom);
		SelectObject(mDC, player);
		Ellipse(mDC,Person.x+5, Person.y- 10, Person.x+15, Person.y);
		RoundRect(mDC, Person.x, Person.y, Person.x + 20, Person.y + 20, 5, 5);
		//�¿�� �̵��ϴ� ���� �׸��� 1-4
		for (int i = 0; i < Amount/2; i++)
		{
			SelectObject(mDC, carline);
			SelectObject(mDC, Car[i].color);
			RoundRect(mDC, Car[i].x, Car[i].y, Car[i].x + 4 * carsize, Car[i].y + 3 * carsize,20,20);
		}
		//���Ϸ� �̵��ϴ� ���� �׸��� 5-8
		for (int i = Amount / 2; i < 8; i++)
		{
			SelectObject(mDC, carline);
			SelectObject(mDC, Car[i].color);
			RoundRect(mDC, Car[i].x, Car[i].y, Car[i].x + 3 * carsize, Car[i].y + 4 * carsize,20,20);
		}
		//������ �Ѿ�� �κ� �׸���
		if (Car[0].x2 > 0)
		{
			SelectObject(mDC, Car[0].color);
			RoundRect(mDC, 0, Car[0].y, Car[0].x2, Car[0].y + 3 * carsize,20,20);
		}//1������
		if (Car[1].x2 > 0)
		{
			SelectObject(mDC, Car[1].color);
			RoundRect(mDC, 0, Car[1].y, Car[1].x2, Car[1].y + 3 * carsize,20,20);
		}//2������
		if (Car[2].x2 < rt.right)
		{
			SelectObject(mDC, Car[2].color);
			RoundRect(mDC, Car[2].x2, Car[2].y, rt.right, Car[2].y + 3 * carsize,20,20);
		}//3������
		if (Car[3].x2 < rt.right)
		{
			SelectObject(mDC, Car[3].color);
			RoundRect(mDC, Car[3].x2, Car[3].y, rt.right, Car[3].y + 3 * carsize,20,20);
		}//4������
		if (Car[4].y2 > 0)
		{
			SelectObject(mDC, Car[4].color);
			RoundRect(mDC, Car[4].x, 0, Car[4].x + 3 * carsize, Car[4].y2,20,20);
		}//5������
		if (Car[5].y2 > 0)
		{
			SelectObject(mDC, Car[5].color);
			RoundRect(mDC, Car[5].x, 0, Car[5].x + 3 * carsize, Car[5].y2,20,20);
		}//6�� ����
		if (Car[6].y2 < rt.bottom)
		{
			SelectObject(mDC, Car[6].color);
			RoundRect(mDC, Car[6].x, Car[6].y2, Car[6].x + 3 * carsize, rt.bottom,20,20);
		}//7������
		if (Car[7].y2 < rt.bottom)
		{
			SelectObject(mDC, Car[7].color);
			RoundRect(mDC, Car[7].x, Car[7].y2, Car[7].x + 3 * carsize, rt.bottom,20,20);
		}//8�� ����

		//--- �������� �޸� DC�� ������ ȭ�� DC�� �����Ѵ�.
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
		DeleteDC(mDC); //--- ������ �޸� DC ����
		DeleteObject(hBitmap); //--- ������ ��Ʈ�� ����
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case Traffic:
			if (green)
			{
				//�������� �̵��ϴ� ����
				for (int i = 0; i < 2; i++) {
					if (Car[i].x <= rt.right ) {
						Car[i].x += 2;
					}
					if (Car[i].x > rt.right - 4 * carsize)
					{
						Car[i].x2 += 2;
					}
					if (Car[i].x >= rt.right)
					{
						Car[i].x = 0;
						Car[i].x2 = 0;
					}
				}
				//�������� �̵��ϴ� ����
				for (int i = 2; i < 4; i++)
				{
						if (Car[i].x + 4 * carsize >= rt.top)
						{
							Car[i].x -= 2;
						}
						if (Car[i].x + 4 * carsize <= 0)
						{
							Car[i].x = Car[i].x2;
							Car[i].x2 = rt.right;
						}
						if (Car[i].x < 0)
						{
							Car[i].x2 -= 2;
						}
				}

				//�Ʒ��� �̵��ϴ� ����
				for (int i = 4; i < 6; i++) {
					//���ؼ� ���� �ִ� ���
					if (Car[i].y + 4 * carsize < rt.bottom / 3 - carsize)
					{
						if (Car[4].y > Car[5].y + 5 * carsize  || Car[5].y > Car[4].y + 5 * carsize)
						{
							Car[i].y += 2;
						}
					}
					//���ؼ� �Ʒ��ִ� ���
					else if (rt.bottom / 3 < Car[i].y + 4 * carsize)
					{
						Car[i].y += 2;
					}
					//
					if (Car[i].y > rt.bottom)
					{
						Car[i].y = 0;
						Car[i].y2 =0;
					}
					if (Car[i].y > rt.bottom - 4 * carsize)
					{
						Car[i].y2 += 2;
					}
				}
				//���� �̵��ϴ� ����
				for (int i = 6; i < 8; i++){
					//���� ���ؼ� �Ʒ��������
					if (rt.bottom * 2 / 3 + carsize < Car[i].y)
					{
						if (Car[6].y >= Car[7].y + 5 * carsize || Car[7].y >= Car[6].y + 5 * carsize)
						{
							Car[i].y -= 2;
						}
					}
					//���� ���ؼ� ���� �ִ� ��� 
					else if (rt.bottom * 2 / 3 - carsize > Car[i].y)
					{
						Car[i].y -= 2;
					}
					if (Car[i].y2 + 4 * carsize < rt.bottom)
					{
						Car[i].y = Car[i].y2;
						Car[i].y2 = rt.bottom;
					}
					if (Car[i].y < 0)
					{
						Car[i].y2 -= 2;
					}

				}
				// �¿�� �̵��ϴ� ���
					//�·� �̵�
					if (Person.y < rt.bottom / 2)
					{
						if (Person.x > rt.right / 3 - 20)
						{
							Person.x-=2;
						}
					}
					//��� �̵�
					else if (Person.y > rt.bottom / 2)
					{
						if (Person.x < rt.right * 2 / 3 + 20)
						{
							Person.x += 2;
						}
					}
			}
			if (red)
			{
				//�������� �̵��ϴ� ����
				for (int i = 0; i < 2; i++) {
					//���ؼ� ���� �ִ� ���
					if (Car[i].x + 4 * carsize < rt.right / 3 - carsize)
					{
						if (Car[1].x > Car[0].x + 5 * carsize || Car[0].x > Car[1].x + 5 * carsize)
						{
							Car[i].x += 2;
						}
					}
					//���ؼ� �Ʒ��ִ� ���
					else if (rt.right / 3 < Car[i].x + 4 * carsize)
					{
						Car[i].x += 2;
					}
					//
					if (Car[i].x > rt.right)
					{
						Car[i].x = 0;
						Car[i].x2 = 0;
					}
					if (Car[i].x > rt.right - 4 * carsize)
					{
						Car[i].x2 += 2;
					}
				}
				//�������� �̵��ϴ� ����
				for (int i = 2; i < 4; i++)
				{
					//���� ���ؼ� �Ʒ��������
					if (rt.right * 2 / 3 + carsize < Car[i].x)
					{
						if (Car[2].x >= Car[3].x + 5 * carsize || Car[3].x >= Car[2].x + 5 * carsize)
						{
							Car[i].x -= 2;
						}
					}
					//���� ���ؼ� ���� �ִ� ��� 
					else if (rt.right * 2 / 3 - carsize > Car[i].x)
					{
						Car[i].x -= 2;
					}
					if (Car[i].x2 + 4 * carsize < rt.right)
					{
						Car[i].x = Car[i].x2;
						Car[i].x2 = rt.right;
					}
					if (Car[i].x < 0)
					{
						Car[i].x2 -= 2;
					}
				}
				//�Ʒ��� �̵��ϴ� ����
				for (int i = 4; i < 6; i++) {
					if (Car[i].y < rt.bottom)
					{
						Car[i].y += 2;
					}
					if (Car[i].y > rt.bottom - 4 * carsize)
					{
						Car[i].y2 += 2;
					}
					if (Car[i].y >= rt.bottom)
					{
						Car[i].y = 0;
						Car[i].y2 = 0;
					}
				}
				//���� �̵��ϴ� ����
				for (int i = 6; i < 8; i++) {
					if (Car[i].y + 4 * carsize >= rt.top)
					{
							Car[i].y -= 2;
					}
					if (Car[i].y + 4 * carsize <= 0)
					{
						Car[i].y = Car[i].y2;
						Car[i].y2 = rt.bottom;
					}
					if (Car[i].y < 0)
					{
						Car[i].y2 -= 2;
					}
				}
				//���� �̵�
				if (Person.x > rt.right / 2)
				{
					if (Person.y > rt.bottom / 3 - 20)
					{
						Person.y-= 2;
					}
				}
				//�Ʒ��� �̵�
				if (Person.x < rt.right / 2)
				{
					if (Person.y < rt.bottom * 2 / 3 + 20)
					{
						Person.y += 2;
					}
				}
			}
		break;
		case SignalLight:
			if (green)
			{
				timecount1++;
				

				if (timecount1 == 10)
				{
					timecount1 = 0;
					green = true;
					yellow = true;
					red = false;
				}
			}
			if (yellow == true && green == true)
			{
				timecount2++;
				SelectObject(mDC, YellowLight);
				Ellipse(mDC, 100, 30, 160, 90);
				if (timecount2 == 3)
				{
					timecount3 = 0;
					timecount2 = 0;
					timecount1 = 0;
					timecount2 = 0;
					green = false;
					yellow = false;
					red = true;
				}

			}
			if (yellow == true && red == true)
			{

				timecount2++;
				if (timecount2 == 3)
				{
					timecount3 = 0;
					timecount2 = 0;
					timecount1 = 0;
					green = true;
					yellow = false;
					red = false;
				}

			}
			if (red)
			{

				timecount3++;
				if (timecount3 == 9)
				{
					green = false;
					yellow = true;
					red = true;
				}
			}
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, false); //--- ȭ�鿡 �ٽñ׸��⸦ �� �� ������ ������ �������� �ʴ´�.
		break;
	case WM_DESTROY:
		DeleteObject(YellowLight);
		DeleteObject(RedLight);
		DeleteObject(GreenLight);
		DeleteObject(BlackLight);
		DeleteObject(ClickBoard);
		DeleteObject(carline);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}