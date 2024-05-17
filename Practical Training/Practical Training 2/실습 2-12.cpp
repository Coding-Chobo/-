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
    int x;
    int y;
    int shape;
    HBRUSH color;
    int size;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    TCHAR Clear1[100];
    TCHAR Clear2[100];
    TCHAR Clear3[100];
    static SIZE size;
    HPEN select, common;
    static bool changeshape = false;
    static int SaveShape[10];
    static figure tenfg[10];
    static HBRUSH CColor[4];
    static int linesize = 20;
    static int mapsize = 40;
    int rndx = rand() % mapsize;
    int rndy = rand() % mapsize;
    int rndc = rand() % 4;
    static int choosenumber;
    static int figurecount{ 0 };
    POINT triangle[3];
    select = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    common = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        static HBRUSH rnd1 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd2 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd3 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd4 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        CColor[0] = rnd1;
        CColor[1] = rnd2;
        CColor[2] = rnd3;
        CColor[3] = rnd4;
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        if (wParam == 's' || wParam == 'S')
        {
            mapsize = 30;
            for (int i = 0; i < 10; i++)
            {
                tenfg[i].shape = 10;
            }
            figurecount = 0;
        }
        else if (wParam == 'm' || wParam == 'M')
        {
            mapsize = 40;
            for (int i = 0; i < 10; i++)
            {
                tenfg[i].shape = 10;
            }
            figurecount = 0;
        }
        else if (wParam == 'l' || wParam == 'L')
        {
            mapsize = 50;
            for (int i = 0; i < 10; i++)
            {
                tenfg[i].shape = 10;
            }
            figurecount = 0;
        }
        else if (wParam == 'e' || wParam == 'E')
        {
            tenfg[figurecount % 10].x = rndx;
            tenfg[figurecount % 10].y = rndy;
            tenfg[figurecount % 10].shape = 0;
            tenfg[figurecount % 10].color = CColor[rndc];
            tenfg[figurecount % 10].size = 0;
            figurecount++;
        }
        else if (wParam == 't' || wParam == 'T')
        {
            tenfg[figurecount % 10].x = rndx;
            tenfg[figurecount % 10].y = rndy;
            tenfg[figurecount % 10].shape = 1;
            tenfg[figurecount % 10].color = CColor[rndc];
            tenfg[figurecount % 10].size = 0;
            figurecount++;
        }
        else if (wParam == 'r' || wParam == 'R')
        {
            tenfg[figurecount % 10].x = rndx;
            tenfg[figurecount % 10].y = rndy;
            tenfg[figurecount % 10].shape = 2;
            tenfg[figurecount % 10].color = CColor[rndc];
            tenfg[figurecount % 10].size = 0;
            figurecount++;
        }
        else if (wParam >= '0' && wParam <= '9')
        {
            if (wParam == '0')
            {
                choosenumber = 10;
            }
            else {
                choosenumber = wParam - '0';
            }

        }

        else if (wParam == '+')
        {
            tenfg[choosenumber - 1].size--;
        }
        else if (wParam == '-')
        {
            tenfg[choosenumber - 1].size++;
        }
        else if (wParam == 'c' || wParam == 'C')
        {
            if (changeshape == false)
            {
                for (int i = 0; ((figurecount < 9 && i < figurecount)) || (figurecount >= 9 && i < 10); i++)
                {
                    SaveShape[i] = tenfg[i].shape;
                    if (tenfg[i].color == tenfg[choosenumber - 1].color) {
                        tenfg[i].shape = 3;
                    }
                }
                changeshape = true;
            }
            else if (changeshape) {
                for (int i = 0; ((figurecount < 9 && i < figurecount)) || (figurecount >= 9 && i < 10); i++)
                {
                    tenfg[i].shape = SaveShape[i];
                }
                changeshape = false;
            }
        }
        else if (wParam == 'p' || wParam == 'P')
        {
            for (int i = 0; i < 10; i++)
            {
                tenfg[i].shape = 10;
            }
            figurecount = 0;
            choosenumber = 0;
        }
        else if (wParam == 'd' || wParam == 'D')
        {
            if ((choosenumber > 0 && figurecount > 0) && (choosenumber <= figurecount))
            {
                if (choosenumber < figurecount % 10)
                {
                    for (int i = choosenumber - 1; i < figurecount%10 - 1; i++) {
                        tenfg[i] = tenfg[i + 1];
                    }
                }
                else if (figurecount % 10 < choosenumber && figurecount % 10 != 0)
                {
                    for (int i = choosenumber - 1; i < figurecount % 10 + 9; i++) {
                        tenfg[i % 10] = tenfg[(i + 1) % 10];
                    }
                }
                else if (figurecount % 10 < choosenumber && figurecount % 10 == 0)
                {
                    for (int i = choosenumber - 1; i < 8; i++) {
                        tenfg[i] = tenfg[i + 1];
                    }
                }
                tenfg[figurecount % 10 - 1].x = 0;
                tenfg[figurecount % 10 - 1].y = 0;
                tenfg[figurecount % 10 - 1].shape = 10;
                tenfg[figurecount % 10 - 1].color = NULL;
                tenfg[figurecount % 10 - 1].size = 0;
                if (choosenumber == figurecount % 10 && figurecount % 10 != 0)
                {
                    figurecount--;
                    choosenumber--;
                }
                else if (choosenumber == figurecount % 10 && figurecount % 10 == 0)
                {
                    choosenumber = 9;
                    figurecount--;
                }
                else
                {
                    figurecount--;
                }
                if (choosenumber == 0)
                {
                    choosenumber = 10;
                }
            }
        }
        else if (wParam == 'q' || wParam == 'Q')
        {
            DeleteObject(rnd1);
            DeleteObject(rnd2);
            DeleteObject(rnd3);
            DeleteObject(rnd4);
            DeleteObject(common);
            DeleteObject(select);
            PostQuitMessage(0);
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        if (wParam == VK_LEFT) {
            if (tenfg[choosenumber - 1].x == 0)
            {
                tenfg[choosenumber - 1].x = 39;
            }
            else {
                tenfg[choosenumber - 1].x--;
            }
        }
        else if (wParam == VK_RIGHT)
        {
            if (tenfg[choosenumber - 1].x==39)
            {
                tenfg[choosenumber - 1].x = 0;
            }
            else {
                tenfg[choosenumber - 1].x++;
            }
        }
        else if (wParam == VK_UP)
        {
            if (tenfg[choosenumber - 1].y == 0)
            {
                tenfg[choosenumber - 1].y = 39;
            }
            else {
                tenfg[choosenumber - 1].y--;
            }
        }
        else if (wParam == VK_DOWN) {
            if (tenfg[choosenumber - 1].y == 39)
            {
                tenfg[choosenumber - 1].y = 0;
            }
            else {
                tenfg[choosenumber - 1].y++;
            }
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 0; i <= mapsize; i++) // 맵 구현
        {
            MoveToEx(hDC, i * linesize, 0, NULL);
            LineTo(hDC, i * linesize, mapsize * linesize);
            MoveToEx(hDC, 0, i * linesize, NULL);
            LineTo(hDC, mapsize * linesize, i * linesize);
        }
        for (int i = 0; ((figurecount < 9 && i < figurecount)) || (figurecount >= 9 && i < 10); i++)
        {
            if (i == choosenumber - 1)
            {
                SelectObject(hDC, select);
            }
            else
            {
                SelectObject(hDC, common);
            }
            switch (tenfg[i].shape)
            {
            case 0:
                SelectObject(hDC, tenfg[i].color);
                Ellipse(hDC, tenfg[i].x * linesize + tenfg[i].size, tenfg[i].y * linesize + tenfg[i].size, (tenfg[i].x + 1) * linesize - tenfg[i].size, (tenfg[i].y + 1) * linesize - tenfg[i].size);
                break;
            case 1:
                SelectObject(hDC, tenfg[i].color);
                triangle[0] = { tenfg[i].x * linesize + linesize / 2,tenfg[i].y * linesize };
                triangle[1] = { tenfg[i].x * linesize,(tenfg[i].y + 1) * linesize };
                triangle[2] = { (tenfg[i].x + 1) * linesize ,(tenfg[i].y + 1) * linesize };
                Polygon(hDC, triangle, 3);
                break;
            case 2: 
                SelectObject(hDC, tenfg[i].color);
                Rectangle(hDC, tenfg[i].x * linesize + tenfg[i].size, tenfg[i].y * linesize + tenfg[i].size, (tenfg[i].x + 1) * linesize - tenfg[i].size, (tenfg[i].y + 1) * linesize - tenfg[i].size);
                break;
            case 3:
                SelectObject(hDC, tenfg[i].color);
                Pie(hDC, tenfg[i].x * linesize + tenfg[i].size, tenfg[i].y * linesize + tenfg[i].size, (tenfg[i].x + 1) * linesize - tenfg[i].size, (tenfg[i].y + 1) * linesize - tenfg[i].size
                    , tenfg[i].x * linesize + linesize * 2 / 3, tenfg[i].y * linesize + linesize / 3, tenfg[i].x * linesize + linesize * 2 / 3, tenfg[i].y * linesize + linesize * 2 / 3);
                break;
            default:
                break;
            }
        }
        wsprintf(Clear1, L"도형의 개수 : %d", figurecount);
        TextOut(hDC, 840, 300, Clear1, lstrlen(Clear1));
        wsprintf(Clear1, L"선택된 도형의 번호 : %d", choosenumber);
        TextOut(hDC, 840, 320, Clear1, lstrlen(Clear1));
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        DeleteObject(rnd1);
        DeleteObject(rnd2);
        DeleteObject(rnd3);
        DeleteObject(rnd4);
        DeleteObject(common);
        DeleteObject(select);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}