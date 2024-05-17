#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 2-12";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 500, 0, 1020, 1050, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}
struct figure {

    int shape;
    int x;
    int y;
    int x2;
    int y2;
    int size;
};
bool Draw_All = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    TCHAR Clear1[200];
    TCHAR Clear2[200];
    TCHAR Clear3[200];
    TCHAR Clear4[200];
    TCHAR Clear5[200];
    TCHAR Clear6[200];
    static SIZE size;
    static TCHAR str[100];
    static int INDEX{ 0 };
    static figure tenfg[10];
    static int figurecount{ 0 };
    static HPEN fgline[10];
    static HBRUSH fgcolor[10];
    RECT textbox = { 10,850,470,970 };
    RECT infobox = { 490,850,980,970 };
    static int number = 0;
    static int textcount = 0;
    POINT triangle[3];
    POINT pentagon[5];
    static COLORREF RndLineColor[10];
    static COLORREF RndFigureColor[10];
    static HBRUSH rnd;
    static int startposition;
    
    
    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        CreateCaret(hWnd, NULL, 2, 15);
        ShowCaret(hWnd);
        for (int i = 0; i < 10; i++)
        {
            RndLineColor[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
            RndFigureColor[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
        }
        
        static HBRUSH common = CreateSolidBrush(RGB(100, 100, 100));
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        if (wParam == '-')
        {
            if (tenfg[figurecount - 1].size > 1 && tenfg[figurecount - 1].size <= 10) {
                tenfg[figurecount - 1].size--;
            }
            else if (tenfg[figurecount - 1].size == 1)
            {
                tenfg[figurecount - 1].x++;
                tenfg[figurecount - 1].y++;
                tenfg[figurecount - 1].x2--;
                tenfg[figurecount - 1].y2--;
            }
            
        }
        else if (wParam == '+')
        {
            if (tenfg[figurecount - 1].size >= 1 && tenfg[figurecount - 1].size < 10)
            {
                tenfg[figurecount - 1].size++;
            }
            else if (tenfg[figurecount - 1].size == 10)
            {
                tenfg[figurecount - 1].x--;
                tenfg[figurecount - 1].y--;
                tenfg[figurecount - 1].x2++;
                tenfg[figurecount - 1].y2++;
            }
        }
        else if (wParam == 'l' || wParam == 'L')
        {
            COLORREF temp2 = RndLineColor[0];
            for (int i = 0; i < 9; i++)
            {
                RndLineColor[i] = RndLineColor[i + 1];
            }
            RndLineColor[9] = temp2;
        }
        else if (wParam == 'i' || wParam == 'I')
        {
            COLORREF temp = RndFigureColor[0];
            for (int i = 0; i < 9; i++)
            {
                RndFigureColor[i] = RndFigureColor[i + 1];
            }
            RndFigureColor[9] = temp;
        }
        InvalidateRect(hWnd, NULL, true);
        ReleaseDC(hWnd, hDC);
        break;
         break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
         if (wParam >= '0' && wParam <= '9') {
            // 숫자 입력
            str[INDEX++] = wParam;
            str[INDEX] = '\0';
         }
        else if (wParam == VK_BACK && INDEX > 0) {
            // 백스페이스 처리
            INDEX--;
            str[INDEX] = '\0';
        }
        else if (wParam == VK_SPACE) {
            number = _ttoi(str); // 문자열을 숫자로 변환

            // 변수에 숫자 할당
            switch (textcount++) {
            case 0: tenfg[figurecount].shape = number; break;
            case 1: tenfg[figurecount].x = number; break;
            case 2: tenfg[figurecount].y = number; break;
            case 3: tenfg[figurecount].x2 = number; break;
            case 4: tenfg[figurecount].y2 = number; break;
            case 5: tenfg[figurecount].size = number;
            }
            if (textcount == 6)
            {
                textcount = 0;
                figurecount++;
                InvalidateRect(hWnd, NULL, true);
            }
            INDEX = 0;
            ZeroMemory(str, sizeof(str)); // 문자열 초기화
            number = 0; // number 초기화
        }
        else if (wParam == VK_LEFT)
        {
            if (tenfg[figurecount - 1].x > 0 && tenfg[figurecount - 1].x2 > 0)
            {
                tenfg[figurecount - 1].x--;
                tenfg[figurecount - 1].x2--;
            }
        }
        else if (wParam == VK_RIGHT)
        {
            if (tenfg[figurecount - 1].x < 1000 && tenfg[figurecount - 1].x2 < 1000)
            {
                tenfg[figurecount - 1].x++;
                tenfg[figurecount - 1].x2++;
            }
        }
        else if (wParam == VK_DOWN)
        {
            if (tenfg[figurecount - 1].y < 830 && tenfg[figurecount - 1].y2 < 830)
            {
                tenfg[figurecount - 1].y++;
                tenfg[figurecount - 1].y2++;
            }
        }
        else if (wParam == VK_UP)
        {
            if (tenfg[figurecount - 1].y > 0 && tenfg[figurecount - 1].y2 > 0)
            {
                tenfg[figurecount - 1].y--;
                tenfg[figurecount - 1].y2--;
            }
        }
        else if (wParam == 'p' || wParam == 'P')
        {
            figurecount--;
        }
        else if (wParam == 'n' || wParam == 'N')
        {
            figurecount++;
        }
       
        else if (wParam == 'a' || wParam == 'A')
        {
            Draw_All = true;
        }

        InvalidateRect(hWnd, NULL, true);
        ReleaseDC(hWnd, hDC);
        break;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        FrameRect(hDC, &textbox, common);
        FrameRect(hDC, &infobox, common);
        if (Draw_All)
        {
            startposition = 0;
        }
        else if (Draw_All == false)
        {
            startposition = figurecount - 1;
        }

        for (int i = startposition; i < figurecount; i++)
        {
                fgline[i] = CreatePen(PS_SOLID, tenfg[i].size*2, RndLineColor[i]);
                fgcolor[i] = CreateSolidBrush(RndFigureColor[i]);
            switch (tenfg[i].shape)
            {
            case 1:
                SelectObject(hDC, fgline[i]);
                SelectObject(hDC, fgcolor[i]);
                MoveToEx(hDC, tenfg[i].x, tenfg[i].y, NULL);
                LineTo(hDC, tenfg[i].x, tenfg[i].y);
                break;
            case 2:
                SelectObject(hDC, fgline[i]);
                SelectObject(hDC, fgcolor[i]);
                MoveToEx(hDC, tenfg[i].x, tenfg[i].y, NULL);
                LineTo(hDC, tenfg[i].x2, tenfg[i].y2);
                break;
            case 3:
                SelectObject(hDC, fgline[i]);
                SelectObject(hDC, fgcolor[i]);
                triangle[0] = { (tenfg[i].x + tenfg[i].x2) / 2,tenfg[i].y };
                triangle[1] = { tenfg[i].x,tenfg[i].y2 };
                triangle[2] = { tenfg[i].x2,tenfg[i].y2 };
                Polygon(hDC, triangle, 3);
                break;
            case 4:
                SelectObject(hDC, fgline[i]);
                SelectObject(hDC, fgcolor[i]);
                Rectangle(hDC, tenfg[i].x, tenfg[i].y, tenfg[i].x2, tenfg[i].y2);
                break;
            case 5:
                SelectObject(hDC, fgline[i]);
                SelectObject(hDC, fgcolor[i]);
                pentagon[0] = { (tenfg[i].x + tenfg[i].x2)/2,tenfg[i].y };
                pentagon[1] = { tenfg[i].x2,tenfg[i].y + max((tenfg[i].y2 - tenfg[i].y),(tenfg[i].y - tenfg[i].y2)) / 3 };
                pentagon[2] = { tenfg[i].x + max((tenfg[i].x2 - tenfg[i].x),(tenfg[i].x - tenfg[i].x2)) * 2 / 3,tenfg[i].y2 };
                pentagon[3] = { tenfg[i].x + max((tenfg[i].x2 - tenfg[i].x),(tenfg[i].x - tenfg[i].x2)) / 3,tenfg[i].y2 };
                pentagon[4] = { tenfg[i].x,tenfg[i].y + max((tenfg[i].y2 - tenfg[i].y),(tenfg[i].y - tenfg[i].y2)) / 3 };
                Polygon(hDC, pentagon, 5);
                break;
            case 6:
                break;

            }
        }
        TextOut(hDC, 20, 940, str, lstrlen(str));
        GetTextExtentPoint32(hDC, str, INDEX, &size);
        SetCaretPos(size.cx + 20, 940);
        wsprintf(Clear1, L"그려질 도형의 번호 : %d   |   모양 : %d", figurecount+1, tenfg[figurecount].shape);
        TextOut(hDC, 40, 860, Clear1, lstrlen(Clear1));
        wsprintf(Clear2, L"X의 값 : %d   |   Y의 값 : %d   |   X2의 값 : %d   |   Y2의 값 : %d",tenfg[figurecount].x, tenfg[figurecount].y, tenfg[figurecount].x2, tenfg[figurecount].y2);
        TextOut(hDC, 40, 880, Clear2, lstrlen(Clear2));
        wsprintf(Clear3, L"두께 : %d",  tenfg[figurecount].size);
        TextOut(hDC, 40, 900, Clear3 ,lstrlen(Clear3));

        wsprintf(Clear4, L"그려진 도형의 번호 : %d   |   모양 : %d", figurecount, tenfg[figurecount-1].shape);
        TextOut(hDC, 500, 860, Clear4, lstrlen(Clear4));
        wsprintf(Clear5, L"X의 값 : %d   |   Y의 값 : %d   |   X2의 값 : %d   |   Y2의 값 : %d", tenfg[figurecount-1].x, tenfg[figurecount-1].y, tenfg[figurecount-1].x2, tenfg[figurecount-1].y2);
        TextOut(hDC, 500, 880, Clear5, lstrlen(Clear5));
        wsprintf(Clear6, L"두께 : %d", tenfg[figurecount-1].size);
        TextOut(hDC, 500, 900, Clear6, lstrlen(Clear6));

        ShowCaret(hWnd);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();

        DeleteObject(common);

        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}