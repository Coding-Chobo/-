#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 2-10";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 500, 50, 820, 850, NULL, NULL, hInstance, NULL);
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
};
int playersize = 1 + rand() % 3;
int goalsize = 1 + rand() % 3;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    static int linesize = 20;
    static int Linecount{ 0 };
    static int Index{ 0 };
    static figure red[4] = { {500,60},{100,100},{300,500},{20,700} };
    static figure colorchange[4] = { {500,500},{240,320},{700,500},{400,700} };
    static figure shapechange[4] = { {40,220},{300,280},{640,320},{360,720} };
    static figure smallsize[4] = { {600,500},{420,240},{360,120},{60,120} };
    static figure largesize[4] = { {500,580},{200,340},{700,280},{280,560} };
    static SIZE size;
    static TCHAR winningmessage[100] = L"이겼습니다~ 축하합니다~ 드디어 끝났어요!!";
    static TCHAR losingmessage[100] = L"그것도 못하냐, 다시 해봐";
    static HBRUSH CColor[4];
    POINT triangle[3] = { { Index * linesize + linesize / 2 , Linecount * linesize + 2 * playersize},
        { Index * linesize + 2 * playersize, (Linecount + 1) * linesize - 2 * playersize },
         { (Index + 1) * linesize - 2 * playersize, (Linecount + 1) * linesize - 2 * playersize }
    };
    POINT goalpoint[3] = { { 790,780+2*goalsize }, { 780 + 2 * goalsize,800 - 2 * goalsize }, { 800 - 2 * goalsize,800 - 2 * goalsize } };
    POINT ChangeTriangle[3] = { {shapechange[3].x + linesize / 2,shapechange[3].y } ,{ shapechange[3].x ,shapechange[3].y + linesize }, { shapechange[3].x + linesize ,shapechange[3].y + linesize } };
    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        static HBRUSH RedBlock = CreateSolidBrush(RGB(255, 0, 0));
        static HBRUSH ChangeShape = CreateSolidBrush(RGB(0, 255, 0));
        static HBRUSH Changelarge = CreateSolidBrush(RGB(200, 200, 200));
        static HBRUSH Changesmall = CreateSolidBrush(RGB(255, 200, 200));
        static HBRUSH rnd1 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd2 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd3 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH rnd4 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        CColor[0] = rnd1;
        CColor[1] = rnd2;
        CColor[2] = rnd3;
        CColor[3] = rnd4;
        static int playerrndcolor = rand() % 4;
        static int goalrndcolor = rand() % 4;
        static HBRUSH playercolor = CColor[playerrndcolor];
        static HBRUSH goalcolor = CColor[goalrndcolor];
        static int playershape = rand() % 4;
        static int goalshape = rand() % 4;
        
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        if (wParam == VK_LEFT)
        {

            for (int i = 0; i < 4; i++)
            {
                if ((Index - 1) * linesize == red[i].x && Linecount * linesize == red[i].y) // red블럭 구현
                {
                    Index++;
                }
                if (Index * linesize == smallsize[i].x && (Linecount)*linesize == smallsize[i].y)   //크기감소 블럭 구현
                {
                    if (playersize > 1)
                    {
                        playersize--;
                    }
                }
                if (Index * linesize == largesize[i].x && (Linecount)*linesize == largesize[i].y)   // 크기증가 블럭 구현
                {
                    if (playersize < 3)
                    {
                        playersize++;
                    }
                }
                if (Index * linesize == colorchange[i].x && (Linecount)*linesize == colorchange[i].y)   // 색상변경 블럭 구현
                {
                    playercolor = CColor[i];
                }
            }
            if (Index * linesize == shapechange[0].x&& Linecount * linesize == shapechange[0].y)
            {
                playershape = 0;
            }else if (Index * linesize == shapechange[1].x && Linecount * linesize == shapechange[1].y)
            {
                playershape = 1;
            }else if (Index * linesize == shapechange[2].x && Linecount * linesize == shapechange[2].y)
            {
                playershape = 2;
            }else if (Index * linesize == shapechange[3].x && Linecount * linesize == shapechange[3].y)
            {
                playershape = 3;
            }
            if (Index == 0)
            {
                Index = 39;
            }
            else {
                Index--;
            }
            
        }
        else if (wParam == VK_RIGHT) {
            for (int i = 0; i < 4; i++)
            {
                if ((Index + 1) * linesize == red[i].x && Linecount * linesize == red[i].y)// red블럭 구현
                {
                    Index--;
                }
                if (Index * linesize == smallsize[i].x && (Linecount)*linesize == smallsize[i].y)   //크기감소 블럭 구현
                {
                    if (playersize > 1)
                    {
                        playersize--;
                    }
                }
                if (Index * linesize == largesize[i].x && (Linecount)*linesize == largesize[i].y)   // 크기증가 블럭 구현
                {
                    if (playersize < 3)
                    {
                        playersize++;
                    }
                }
                if (Index * linesize == colorchange[i].x && (Linecount)*linesize == colorchange[i].y)   // 색상변경 블럭 구현
                {
                    playercolor = CColor[i];
                }
                if (Index * linesize == shapechange[0].x && Linecount * linesize == shapechange[0].y)
                {
                    playershape = 0;
                }
                else if (Index * linesize == shapechange[1].x && Linecount * linesize == shapechange[1].y)
                {
                    playershape = 1;
                }
                else if (Index * linesize == shapechange[2].x && Linecount * linesize == shapechange[2].y)
                {
                    playershape = 2;
                }
                else if (Index * linesize == shapechange[3].x && Linecount * linesize == shapechange[3].y)
                {
                    playershape = 3;
                }
            }
            if (Index == 39)
            {
                Index = 0;
            }
            else {
                Index++;
            }
        }
        else if (wParam == VK_UP) {
            for (int i = 0; i < 4; i++)
            {
                if (Index * linesize == red[i].x && (Linecount-1) * linesize == red[i].y)   // 레드블럭 기능 구현
                {
                    Linecount++;
                }
                if (Index * linesize == smallsize[i].x && (Linecount)*linesize == smallsize[i].y)   //크기감소 블럭 구현
                {
                    if (playersize > 1)
                    {
                        playersize--;
                    }
                }
                if (Index * linesize == largesize[i].x && (Linecount)*linesize == largesize[i].y)   // 크기증가 블럭 구현
                {
                    if (playersize < 3)
                    {
                        playersize++;
                    }
                }
                if (Index * linesize == colorchange[i].x && (Linecount)*linesize == colorchange[i].y)   // 색상변경 블럭 구현
                {
                    playercolor = CColor[i];
                }
                if (Index * linesize == shapechange[0].x && Linecount * linesize == shapechange[0].y)
                {
                    playershape = 0;
                }
                else if (Index * linesize == shapechange[1].x && Linecount * linesize == shapechange[1].y)
                {
                    playershape = 1;
                }
                else if (Index * linesize == shapechange[2].x && Linecount * linesize == shapechange[2].y)
                {
                    playershape = 2;
                }
                else if (Index * linesize == shapechange[3].x && Linecount * linesize == shapechange[3].y)
                {
                    playershape = 3;
                }
            }
            
            if (Linecount==0)
            {
                Linecount = 39;
            }
            else {
                Linecount--;
            }
        }
        else if (wParam == VK_DOWN) {
            for (int i = 0; i < 4; i++)
            {
                if (Index * linesize == red[i].x && (Linecount + 1) * linesize == red[i].y) // red블럭 구현
                {
                    Linecount--;
                }
                if (Index * linesize == smallsize[i].x && (Linecount)*linesize == smallsize[i].y)   //크기감소 블럭 구현
                {
                    if (playersize > 1)
                    {
                        playersize--;
                    }
                }
                if (Index * linesize == largesize[i].x && (Linecount)*linesize == largesize[i].y)   // 크기증가 블럭 구현
                {
                    if (playersize < 3)
                    {
                        playersize++;
                    }
                }
                if (Index * linesize == colorchange[i].x && (Linecount)*linesize == colorchange[i].y)   // 색상변경 블럭 구현
                {
                    playercolor = CColor[i];
                }
                if (Index * linesize == shapechange[0].x && Linecount * linesize == shapechange[0].y)
                {
                    playershape = 0;
                }
                else if (Index * linesize == shapechange[1].x && Linecount * linesize == shapechange[1].y)
                {
                    playershape = 1;
                }
                else if (Index * linesize == shapechange[2].x && Linecount * linesize == shapechange[2].y)
                {
                    playershape = 2;
                }
                else if (Index * linesize == shapechange[3].x && Linecount * linesize == shapechange[3].y)
                {
                    playershape = 3;
                }
            }
            if (Linecount == 39)
            {
                Linecount = 0;
            }
            else {
                Linecount++;
            }
           
        }
        

        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_KEYUP:
        hDC = GetDC(hWnd);
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 0; i <= 40; i++) // 맵 구현
        {
            MoveToEx(hDC, i * linesize, 0, NULL);
            LineTo(hDC, i * linesize, 800);
            MoveToEx(hDC, 0, i * linesize, NULL);
            LineTo(hDC, 800, i * linesize);
        }
        for (int i = 0; i < 4; i++) // 레드블럭 생성
        {
            SelectObject(hDC, RedBlock);
            Rectangle(hDC, red[i].x, red[i].y, red[i].x+linesize, red[i].y+linesize);
        }
        for (int i = 0; i < 4; i++) // 색바꿈 블럭 생성 (랜덤색 4개)
        {
            SelectObject(hDC, CColor[i]);
            Rectangle(hDC, colorchange[i].x, colorchange[i].y, colorchange[i].x + linesize, colorchange[i].y + linesize);
        }
        SelectObject(hDC, ChangeShape);
        Ellipse(hDC, shapechange[0].x, shapechange[0].y, shapechange[0].x + linesize, shapechange[0].y + linesize);
        Rectangle(hDC, shapechange[1].x + 2, shapechange[1].y + 2, shapechange[1].x - 2 + linesize, shapechange[1].y - 2 + linesize);
        Pie(hDC, shapechange[2].x, shapechange[2].y, shapechange[2].x + linesize, shapechange[2].y + linesize, 
            shapechange[2].x + linesize / 2, shapechange[2].y, shapechange[2].x + linesize, shapechange[2].y + linesize / 2);
        Polygon(hDC, ChangeTriangle, 3);
        for (int i = 0; i < 4; i++) // 크기감소 블럭 생성 (회색)
        {
            SelectObject(hDC, Changesmall);
            Rectangle(hDC, smallsize[i].x, smallsize[i].y, smallsize[i].x + linesize, smallsize[i].y + linesize);
        }
        for (int i = 0; i < 4; i++) // 크기증가 블럭 생성 (핑크)
        {
            SelectObject(hDC, Changelarge);
            Rectangle(hDC, largesize[i].x, largesize[i].y, largesize[i].x + linesize, largesize[i].y + linesize);
        }
        //플레이어 그리기
        switch (playershape) 
        {
        case 0: //원
            SelectObject(hDC, playercolor);
            Ellipse(hDC, Index * linesize + 2 * playersize, Linecount * linesize + 2 * playersize, (Index + 1) * linesize - 2 * playersize, (Linecount + 1) * linesize - 2 * playersize);
            break;
        case 1: //사각형
            SelectObject(hDC, playercolor);
            Rectangle(hDC, Index * linesize + 2 * playersize, Linecount * linesize + 2 * playersize, (Index + 1) * linesize - 2 * playersize, (Linecount + 1) * linesize - 2 * playersize);
            break;
        case 2: // 파이
            SelectObject(hDC, playercolor);
            Pie(hDC, Index * linesize + 2 * playersize, Linecount * linesize + 2 * playersize, (Index + 1) * linesize - 2 * playersize, (Linecount + 1) * linesize - 2 * playersize,
                Index * linesize + linesize / 2, Linecount * linesize + 2 * playersize, (Index + 1) * linesize - 2 * playersize, Linecount * linesize + linesize / 2);
            break;
        case 3: // 삼각형
            SelectObject(hDC, playercolor);
            Polygon(hDC, triangle, 3);
            break;

        default:
            break;
        }
        //목표지점 그리기
        switch (goalshape)
        {
        case 0: // 원
            SelectObject(hDC, goalcolor);
            Ellipse(hDC, 780 + goalsize * 2, 780 + 2 * goalsize, 800 - 2 * goalsize, 800 - 2 * goalsize);
            break;
        case 1: // 네모
            SelectObject(hDC, goalcolor);
            Rectangle(hDC, 780 + goalsize * 2, 780 + 2 * goalsize, 800 - 2 * goalsize, 800 - 2 * goalsize);
            break;
        case 2: // 파이
            SelectObject(hDC, goalcolor);
            Pie(hDC, 780 + goalsize * 2, 780 + 2 * goalsize, 800 - 2 * goalsize, 800 - 2 * goalsize
                , 790, 780 + 2 * goalsize, 800 - 2 * goalsize, 790);
            break;
        case 3: // 삼각형
            SelectObject(hDC, goalcolor);
            Polygon(hDC, goalpoint, 3);
            break;
        default:
            break;
        }
        //목표 지점 도착시 메세지 출력
        if (Index == 39 && Linecount == 39)
        {
            if ((playershape == goalshape) && (playercolor == goalcolor) && (playersize == goalsize))
            {
                TextOut(hDC, 380, 400, winningmessage, lstrlen(winningmessage));

            }
            else {
                TextOut(hDC, 380, 400, losingmessage, lstrlen(losingmessage));
            }
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        DeleteObject(RedBlock);
        DeleteObject(rnd1);
        DeleteObject(rnd2);
        DeleteObject(rnd3);
        DeleteObject(rnd4);
        DeleteObject(Changelarge);
        DeleteObject(Changesmall);
        DeleteObject(ChangeShape);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}