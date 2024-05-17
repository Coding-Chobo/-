#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <cmath>
#include "resource1.h"

HINSTANCE g_hInst;

LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 3-2";
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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);   
    RegisterClassEx(&WndClass);
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 800, 300, 600, 500, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}
#define MAX 10
#define LINE 3
#define Status 3
#define BSIZE 10 //--- 반지름
#define Bounce 100
#define Basic 40

//--- (x1, y1)과 (x2, y2)간의 길이
float LengthPts(int x1, int y1, int x2, int y2)
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
// 원과 점 충돌검사
BOOL InCircle(int x1, int y1, int x2, int y2)
{
    if (LengthPts(x1, y1, x2, y2) < BSIZE) //--- BSIZE: 반지름
        return TRUE;
    else
        return FALSE;
}
struct figure {
    int x;
    int y;
    int size;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hDC, mDC;
    static HBITMAP hBitmap;
    static RECT rt;
    TCHAR Clear1[200];
    TCHAR Clear2[200];
    TCHAR Clear3[200];
    TCHAR Clear4[200];
    TCHAR Clear5[200];
    static SIZE size;
    static int Index{ 0 };
    static int LineCount{ 0 };
    static int touch1count{ 0 }, touch2count{ 0 };
    static bool StopBounce = false;
    static int blocksize = 20;
    static int Mapsize = 40;
    static int rndSpeed = (rand() % 3 + 1) * 5;
    static int ChangeSpeed = 0;
    static bool left = true, right = false, iscrash = false;
    static bool down = true, up = false, FL = false, FR = false;
    int mx, my, mx2, my2;
    static bool mod1 = false, mod2 = false, mod3 = false;
    static figure Ball{ 300,10,10 };
    static figure Block[Status][LINE][MAX];
    
    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rt);

        static HBRUSH FirstColor = CreateSolidBrush(RGB(255, 0, 0));
        static HBRUSH SecondColor = CreateSolidBrush(RGB(255, 120, 120));
        static HBRUSH BallColor = CreateSolidBrush(RGB(0, 120, 120));
        //////////////////블록 초기값 설정
        for (int i = 0; i < LINE; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                Block[0][i][j].x = 50 + j * blocksize*2;
                Block[0][i][j].y = 400 + i * blocksize;
                Block[1][i][j].x = 500;
                Block[1][i][j].y = 600;
            }
        }
        SetTimer(hWnd, Basic, 50, NULL);
        break;
    case WM_LBUTTONDOWN:
        if (Block[0][0][0].x > 20)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < LINE; i++)
                {
                    for (int j = 0; j < MAX; j++)
                    {
                        Block[k][i][j].x -= 10;
                    }
                }
            }
        }
        break;
    case WM_RBUTTONDOWN:

        if (Block[0][0][MAX - 1].x + 2 * blocksize <= rt.right-20)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < LINE; i++)
                {
                    for (int j = 0; j < MAX; j++)
                    {
                        Block[k][i][j].x += 10;
                    }
                }
            }
        }
        break;
    case WM_CHAR:
        if (wParam == 's' || wParam == 'S')
        {
            switch (rndSpeed)
            {
            case 5:
                mod1 = true;
                mod2 = false;
                mod3 = false;
                break;
            case 10:
                mod1 = false;
                mod2 = false;
                mod3 = true;
                break;
            case 15:
                mod1 = false;
                mod2 = false;
                mod3 = true;
                break;
            default:
                break;
            }
            ChangeSpeed = rndSpeed;
            SetTimer(hWnd, Bounce, ChangeSpeed, NULL);
        }
        else if (wParam == 'p' || wParam == 'P')
        {
            if (StopBounce)
            {
                SetTimer(hWnd, Bounce, ChangeSpeed, NULL);
                StopBounce = false;

            }
            else {
                StopBounce = true;
                KillTimer(hWnd, Bounce);
            }
        }
        else if (wParam == 'n' || wParam == 'N')
        {
            KillTimer(hWnd, Bounce);
            Ball.x = 300, Ball.y = 10;
            touch1count = 0;
            touch2count = 0;
            for (int i = 0; i < LINE; i++)
            {
                for (int j = 0; j < MAX; j++)
                {
                    Block[0][i][j].x = 50 + j * blocksize * 2;
                    Block[0][i][j].y = 400 + i * blocksize;
                    Block[1][i][j].x = 500;
                    Block[1][i][j].y = 600;
                }
            }
            SetTimer(hWnd, Basic, 50, NULL);
            
        } 
        else if (wParam == 'q' || wParam == 'Q')
        {
            DeleteObject(BallColor);
            DeleteObject(FirstColor);
            DeleteObject(SecondColor);
            DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
            DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
            PostQuitMessage(0);
        }
        else if (wParam == '+')
        {
            if (ChangeSpeed >1)
            {
                ChangeSpeed--;
            }
            
        
            SetTimer(hWnd, Bounce, ChangeSpeed, NULL);
        }
        else if (wParam == '-')
        {
                ChangeSpeed++;
            SetTimer(hWnd, Bounce, ChangeSpeed, NULL);
        }
        break;
    case WM_PAINT:
        GetClientRect(hWnd, &rt); //--- 윈도우 크기 얻어오기
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC); //--- 메모리 DC 만들기
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
        SelectObject(mDC, (HBITMAP)hBitmap); //--- 메모리 DC와 비트맵 연결하기
        Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- 화면에 비어있기 때문에 화면 가득히 사각형을 그려 배경색으로 설정하기
        //기본 블럭들 그리기
        for (int stat = 0; stat < Status; stat++)
        {
            for (int i = 0; i < LINE; i++)
            {
                for (int j = 0; j < MAX; j++)
                {
                    SelectObject(mDC, FirstColor);
                    Rectangle(mDC, Block[0][i][j].x, Block[0][i][j].y, Block[0][i][j].x + 2 * blocksize, Block[0][i][j].y + blocksize);
                }
            }
        }
        //한번 부딪힌 블럭 그리기
        if (touch1count>0)
        {
            for (int stat = 0; stat < Status; stat++)
            {
                for (int i = 0; i < LINE; i++)
                {
                    for (int j = 0; j < MAX; j++)
                    {
                        SelectObject(mDC, SecondColor);
                        Rectangle(mDC, Block[1][i][j].x, Block[1][i][j].y, Block[1][i][j].x + 2 * blocksize, Block[1][i][j].y + blocksize);
                    }
                }
            }
        }
        //공 그리기
        SelectObject(mDC, BallColor);
        Ellipse(mDC, Ball.x-Ball.size, Ball.y- Ball.size, Ball.x + Ball.size, Ball.y + Ball.size);

        wsprintf(Clear1, L"ChangeSpeed : %d",ChangeSpeed);
        TextOut(mDC, 100, 100, Clear1, lstrlen(Clear1));

        // 퍼즈상태일때 벽돌의 현 상황호출
        if (StopBounce)
        {
            wsprintf(Clear4, L"색이 변한 벽돌의 개수 : %d", touch1count);
            TextOut(mDC, 200, 120, Clear4, lstrlen(Clear4));
            wsprintf(Clear5, L"없어진 벽돌의 개수 : %d", touch2count);
            TextOut(mDC, 200, 140, Clear5, lstrlen(Clear5));
        }
        //공과 블럭 충돌검사
        if (down)
        {
            for (int i = 0; i < LINE; i++)
            {
                for (int j = 0; j < MAX; j++)
                {
                    if (Block[0][i][j].y == Ball.y + Ball.size && (Ball.x >= Block[0][i][j].x && Ball.x <= Block[0][i][j].x + 2 * blocksize))
                    {
                        Block[1][i][j].x = Block[0][i][j].x;
                        Block[1][i][j].y = Block[0][i][j].y;
                        touch1count++;
                        Block[0][i][j].x = 300;
                        Block[0][i][j].y = 700;
                        down = false;
                        FL = true;
                    }
                    else if (Block[1][i][j].y == Ball.y + Ball.size && (Ball.x >= Block[1][i][j].x && Ball.x <= Block[1][i][j].x + 2 * blocksize))
                    {
                        Block[2][i][j].x = Block[1][i][j].x;
                        Block[2][i][j].y = Block[1][i][j].y;
                        touch2count++;;
                        Block[1][i][j].y = 700;
                        down = false;
                        FL = true;
                    }
                }
            }
        }
        //공과 좌측면 충돌검사
        if (Ball.x - Ball.size <= 0)
        {
            FL = false;
            FR = true;
        }
        //공과 우측면 충돌검사
        if (Ball.x + Ball.size >= rt.right)
        {
            FR = false;
            FL = true;
        }
        //공과 천장 충돌검사
        else if (Ball.y - Ball.size <= 0)
        {
            FL = false;
            FR = false;
            up = false;
            down = true;
            int ChangeMode = rand() % 3;
            switch (ChangeMode)
            {
            case 0:
                mod1 = false;
                mod2 = false;
                mod3 = true;
                break;
            case 1:
                mod1 = false;
                mod2 = true;
                mod3 = true;
                break;
            case 2:
                mod1 = true;
                mod2 = false;
                mod3 = false;
                break;
            default:
                break;
            }
        }
        //공과 바닥 충돌검사
        else if (Ball.y + Ball.size >= rt.bottom)
        {
            Ball.y = 10;
        }
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case Bounce:
            if (mod1)
            {
                if (down)
                {
                    Ball.y += 2;
                }
                if (up)
                {
                    Ball.y -= 2;
                }
                if (FR)
                {
                    Ball.x += 2;
                    Ball.y -= 2;
                }
                if (FL)
                {
                    Ball.x -= 2;
                    Ball.y -= 2;
                }

            }
            else if (mod2)
            {
                if (down)
                {
                    Ball.y += 2;
                }
                if (up)
                {
                    Ball.y -= 2;
                }
                if (FR)
                {
                    Ball.x += 4;
                    Ball.y -= 2;
                }
                if (FL)
                {
                    Ball.x -= 4;
                    Ball.y -= 2;
                }
            }
            else if (mod3) {
                if (down)
                {
                    Ball.y += 2;
                }
                if (up)
                {
                    Ball.y -= 2;
                }
                if (FR)
                {
                    Ball.x += 2;
                    Ball.y -= 4;
                }
                if (FL)
                {
                    Ball.x -= 2;
                    Ball.y -= 4;
                }
            }
            InvalidateRect(hWnd, NULL, false);
            break;
         case Basic:
            if (left)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int i = 0; i < LINE; i++)
                    {
                        for (int j = 0; j < MAX; j++)
                        {
                            Block[k][i][j].x--;
                            if (Block[0][i][0].x <= 0)
                            {
                                Block[0][i][j].x++;
                                left = false;
                                right = true;
                            }
                        }
                    }
                }


                
            }
            else if (right)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int i = 0; i < LINE; i++)
                    {
                        for (int j = 0; j < MAX; j++)
                        {
                            Block[k][i][j].x++;
                            if (Block[0][i][MAX - 1].x + 2 * blocksize >= rt.right)
                            {
                                Block[0][i][j].x--;
                                left = true;
                                right = false;
                            }
                        }
                    }
                }
                    
                

            }
            InvalidateRect(hWnd, NULL, false);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, Basic);
        KillTimer(hWnd, Bounce);
        DeleteObject(FirstColor);
        DeleteObject(SecondColor);
        DeleteObject(BallColor);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
