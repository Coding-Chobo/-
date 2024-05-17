#include <windows.h>
#include <tchar.h>
#include <time.h>

#include <math.h >
HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 3-5";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 1000, 200, 800, 600, NULL, NULL, hInstance, NULL);
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
    int size;
    bool Drag;
    HBRUSH color;
    HBRUSH color2;
    double theta;
    double theta2;
    bool mod;
};
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
// 원과 점 충돌검사
BOOL InCircle(int x1, int y1, int x2, int y2, figure Rcircle)
{
    if (LengthPts(x1, y1, x2, y2) < Rcircle.size) //--- SIZE: 반지름
        return true;
    else
        return false;
}
typedef struct {
    int x;
    int y;
    int shape;
    int x2;
    int y2;
    bool select;
    HBRUSH color;

}figure;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hDC, mDC;
    static HBITMAP hBitmap;
    static RECT rt;
    static int mapsize{ 40 };
    static int blocksize{ 20 }, blockcount{ 0 };
    static figure block[5];
    static bool selection = false, Drag = false;
    static TCHAR clear[100];

    static int mx, my, mx2, my2;
    switch (iMessage) {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rt);
        static HPEN select = CreatePen(PS_SOLID, 5, RGB(200, 50, 50));
        static HPEN basic = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        for (int i = 0; i < 5; i++)
        {
            block[i].color = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        }
        break;
    case WM_CHAR:
        if (wParam == '1')
        {
            block[0].select = true;
            block[1].select = false;
            block[2].select = false;
            block[3].select = false;
            block[4].select = false;

        }
        else if (wParam == '2')
        {
            block[0].select = false;
            block[1].select = true;
            block[2].select = false;
            block[3].select = false;
            block[4].select = false;
        }
        else if (wParam == '3')
        {
            block[0].select = false;
            block[1].select = false;
            block[2].select = true;
            block[3].select = false;
            block[4].select = false;
        }
        else if (wParam == '4')
        {
            block[0].select = false;
            block[1].select = false;
            block[2].select = false;
            block[3].select = true;
            block[4].select = false;
        }
        else if (wParam == '5')
        {
            block[0].select = false;
            block[1].select = false;
            block[2].select = false;
            block[3].select = false;
            block[4].select = true;
        }
        break;
    case WM_KEYDOWN:
        if (wParam == VK_UP)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].y--;
                    block[i].y2--;
                }
            }
        }
        else if (wParam == VK_DOWN)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].y++;
                    block[i].y2++;
                }
            }
        }
        else if (wParam == VK_RIGHT)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].x++;
                    block[i].x2++;
                }
            }
        }
        else if (wParam == VK_LEFT)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].x--;
                    block[i].x2--;
                }
            }
        }


        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONDOWN:
        if (blockcount < 5)
        {
            selection = true;
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            block[blockcount].x = mx / blocksize;
            block[blockcount].y = my / blocksize;
        }
        break;
    case WM_RBUTTONDOWN:

        break;
    case WM_MOUSEMOVE:
        if (blockcount < 5 && selection)
        {
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            block[blockcount].x2 = mx / blocksize + 1;
            block[blockcount].y2 = my / blocksize + 1;

        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONUP:
        if (blockcount < 5)
        {
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            block[blockcount].x2 = mx / blocksize + 1;
            block[blockcount].y2 = my / blocksize + 1;
            blockcount++;
            selection = false;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC); //--- 메모리 DC 만들기
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
        SelectObject(mDC, (HBITMAP)hBitmap); //--- 메모리 DC와 비트맵 연결하기
        //--- 모든 그리기를 메모리 DC에 한다.

        Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- 화면에 비어있기 때문에 화면 가득히 사각형을 그려 배경색으로 설정하기
        //맵그리기
        for (int i = 0; i <= mapsize; i++)
        {
            for (int j = 0; j <= mapsize; j++)
            {
                MoveToEx(mDC, i * blocksize, j * blocksize, NULL);
                LineTo(mDC, rt.right, j * blocksize);
                MoveToEx(mDC, i * blocksize, j * blocksize, NULL);
                LineTo(mDC, i * blocksize, rt.bottom);
            }
        }
        wsprintf(clear, L"%d", block[0].x2);
        TextOut(mDC, 100, 430, clear, lstrlen(clear));

        //block그리기
        for (int i = 0; i < blockcount + 1; i++)
        {
            if (block[i].select)
            {
                SelectObject(mDC, select);
            }
            SelectObject(mDC, block[i].color);
            Rectangle(mDC, block[i].x * blocksize, block[i].y * blocksize, block[i].x2 * blocksize, block[i].y2 * blocksize);
            SelectObject(mDC, basic);
        }

        //--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}