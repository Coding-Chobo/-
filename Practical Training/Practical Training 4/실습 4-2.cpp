#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <math.h >
#include "resource1.h"
HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 4-3";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 850, 0, 800, 820, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}


typedef struct {
    int shape;
    bool select;
    HBRUSH color;
    RECT rect;
}figure;
bool isOverlap(figure block1, figure block2) {
    if (block1.rect.left < block2.rect.right && block1.rect.right > block2.rect.left &&
        block1.rect.top < block2.rect.bottom && block1.rect.bottom > block2.rect.top)
        return true;
    return false;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hDC, mDC;
    static HBITMAP hBitmap;
    static RECT rt;
    static int mapsize{ 40 };
    static int blocksize{ 20 }, blockcount{ 0 };
    static figure block[5];
    static bool selection = false, Drag = false, dot_mod = false, MoveMode = true, inform = false, border = true;
    static TCHAR Clear[100];
    int mx, my, mx2, my2;
    switch (iMessage) {
    case WM_CREATE:

        srand(time(NULL));
        GetClientRect(hWnd, &rt);
        static HPEN select = CreatePen(PS_SOLID, 5, RGB(200, 50, 50));
        static HPEN basic = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        static HPEN dot = CreatePen(PS_DOT,1, RGB(0, 0, 0));
        static HBRUSH c1 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH c2 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH c3 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH c4 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH c5 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        static HBRUSH c6 = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
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

        if (wParam == VK_UP && MoveMode)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    int xsize = block[i].rect.right - block[i].rect.left;
                    int ysize = block[i].rect.bottom - block[i].rect.top;
                    block[i].rect.top--;
                    block[i].rect.bottom--;
                    if (block[i].rect.top < 0)
                    {
                        block[i].rect.top = rt.bottom / blocksize - xsize;
                        block[i].rect.bottom= rt.bottom / blocksize;
                    }
                }
            }
        }
        else if (wParam == VK_DOWN && MoveMode)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    int xsize = block[i].rect.right - block[i].rect.left;
                    int ysize = block[i].rect.bottom - block[i].rect.top;
                    block[i].rect.top++;
                    block[i].rect.bottom++;
                    if (block[i].rect.bottom * blocksize > rt.bottom)
                    {
                        block[i].rect.top = 0;
                        block[i].rect.bottom = xsize;

                    }
                }
            }
        }
        else if (wParam == VK_RIGHT && MoveMode)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    int xsize = block[i].rect.right - block[i].rect.left;
                    int ysize = block[i].rect.bottom - block[i].rect.top;
                    block[i].rect.right++;
                    block[i].rect.left++;
                    if (block[i].rect.right * blocksize > rt.right)
                    {
                        block[i].rect.left = 0;
                        block[i].rect.right = xsize;

                    }
                }
            }
        }
        else if (wParam == VK_LEFT && MoveMode)
        {
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    int xsize = block[i].rect.right - block[i].rect.left;
                    int ysize = block[i].rect.bottom - block[i].rect.top;
                    block[i].rect.left--;
                    block[i].rect.right--;
                    if (block[i].rect.left  < 0)
                    {
                        block[i].rect.left = rt.right / blocksize - xsize;
                        block[i].rect.right = rt.right / blocksize;
                    }
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
            block[blockcount].rect.left = mx / blocksize;
            block[blockcount].rect.top = my / blocksize;
        }
        break;
    case WM_LBUTTONUP:
        if (blockcount < 5)
        {
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            block[blockcount].rect.right = mx / blocksize + 1;
            block[blockcount].rect.bottom = my / blocksize + 1;
            blockcount++;
            selection = false;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_RBUTTONDOWN:
        mx = LOWORD(lParam) / blocksize;
        my = HIWORD(lParam) / blocksize;
        
        for (int i = 0; i < blockcount; i++)
        {
            if (block[i].rect.left <= mx && block[i].rect.right > mx && block[i].rect.top <= my && block[i].rect.bottom > my) {
                Drag = true;
                for (int j = 0; j < 5; j++)
                {
                    block[j].select = false;
                }
                block[i].select = true;
            }
        }
        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_RBUTTONUP:
        Drag = false;
        break;
    case WM_MOUSEMOVE:
        mDC = GetDC(hWnd);
        mx2 = LOWORD(lParam) / blocksize;
        my2 = HIWORD(lParam) / blocksize;
        if (blockcount < 5 && selection)
        {
            block[blockcount].rect.right = mx2 + 1;
            block[blockcount].rect.bottom = my2 + 1;
        }
        if (Drag && MoveMode)
        {

            for (int i = 0; i < blockcount; i++)
            {
                if (block[i].select)
                {
                    int xsize = block[i].rect.right - block[i].rect.left;
                    int ysize = block[i].rect.bottom - block[i].rect.top;
                    block[i].rect.left = (mx2);
                    block[i].rect.top = (my2);
                    block[i].rect.right = (mx2 + xsize);
                    block[i].rect.bottom= (my2 + ysize);
                    if (block[i].rect.right*blocksize>rt.right)
                    {
                        block[i].rect.left = 0;
                        block[i].rect.right = xsize;

                    }
                    if (block[i].rect.left * blocksize < 0)
                    {
                        block[i].rect.left = rt.right / blocksize - xsize;
                        block[i].rect.right = rt.right / blocksize;
                    }
                }
            }
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
                if (dot_mod && border)
                {
                    SelectObject(mDC, dot);
                }
                MoveToEx(mDC, i * blocksize, j * blocksize, NULL);
                LineTo(mDC, rt.right, j * blocksize);
                MoveToEx(mDC, i * blocksize, j * blocksize, NULL);
                LineTo(mDC, i * blocksize, rt.bottom);
            }
        }

        //block그리기
        for (int i = 0; i < blockcount + 1; i++)
        {
            if (block[i].select&&border)
            {
                SelectObject(mDC, select);
            }
            SelectObject(mDC, block[i].color);
            Rectangle(mDC, block[i].rect.left * blocksize, block[i].rect.top * blocksize, block[i].rect.right * blocksize, block[i].rect.bottom * blocksize);
            SelectObject(mDC, basic);
        }
        // 겹치는 부분에 대해 각 도형의 색을 합쳐서 새로운 색을 생성하여 적용
        for (int i = 0; i < blockcount; i++) {
            for (int j = i + 1; j < blockcount; j++) {
                if (isOverlap(block[i], block[j])) {
                    RECT overlapRect;
                    overlapRect.left = max(block[i].rect.left * blocksize, block[j].rect.left * blocksize);
                    overlapRect.top = max(block[i].rect.top * blocksize, block[j].rect.top * blocksize);
                    overlapRect.right = min(block[i].rect.right * blocksize, block[j].rect.right * blocksize);
                    overlapRect.bottom = min(block[i].rect.bottom * blocksize, block[j].rect.bottom * blocksize);

                    // 두 도형의 색을 합쳐서 새로운 색 생성
                    COLORREF color1 = GetPixel(mDC, block[i].rect.left * blocksize + 5, block[i].rect.top * blocksize + 5);
                    COLORREF color2 = GetPixel(mDC, block[j].rect.left * blocksize + 5, block[j].rect.top * blocksize + 5);
                    int mergedRed = min(GetRValue(color1) + GetRValue(color2), 255);
                    int mergedGreen = min(GetGValue(color1) + GetGValue(color2), 255);
                    int mergedBlue = min(GetBValue(color1) + GetBValue(color2), 255);
                    // 겹친 영역에 새로운 색 적용
                    HBRUSH mergedBrush = CreateSolidBrush(RGB(mergedRed, mergedGreen, mergedBlue));
                    FillRect(mDC, &overlapRect, mergedBrush);

                }
            }
        }
        //inform
        if (inform)
        {
            wsprintf(Clear, L"도형의 개수 : %d", blockcount);
            TextOut(mDC, 200, 300, Clear, lstrlen(Clear));
            wsprintf(Clear, L"1번 도형의 크기 : %d (가로) , %d(세로)", block[0].rect.right - block[0].rect.left, block[0].rect.bottom - block[0].rect.top);
            TextOut(mDC, 200, 320, Clear, lstrlen(Clear));
            wsprintf(Clear, L"2번 도형의 크기 : %d (가로) , %d(세로)", block[1].rect.right - block[1].rect.left, block[1].rect.bottom - block[1].rect.top);
            TextOut(mDC, 200, 340, Clear, lstrlen(Clear));
            wsprintf(Clear, L"3번 도형의 크기 : %d (가로) , %d(세로)", block[2].rect.right - block[2].rect.left, block[2].rect.bottom - block[2].rect.top);
            TextOut(mDC, 200, 360, Clear, lstrlen(Clear));
            wsprintf(Clear, L"4번 도형의 크기 : %d (가로) , %d(세로)", block[3].rect.right - block[3].rect.left, block[3].rect.bottom - block[3].rect.top);
            TextOut(mDC, 200, 380, Clear, lstrlen(Clear));
            wsprintf(Clear, L"5번 도형의 크기 : %d (가로) , %d(세로)", block[4].rect.right - block[4].rect.left, block[4].rect.bottom - block[4].rect.top);
            TextOut(mDC, 200, 400, Clear, lstrlen(Clear));

        }

        //--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_GRID_1:
            dot_mod = false;
            break;
        case ID_GRID_DOT:
            dot_mod = true;
            break;
        case ID_COLOR_1:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c1;
                }
            }
            break;
        case ID_COLOR_2:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c2;
                }
            }
            break;
        case ID_COLOR_3:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c3;
                }
            }
            break;
        case ID_COLOR_4:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c4;
                }
            }
            break;
        case ID_COLOR_5:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c5;
                }
            }
            break;
        case ID_COLOR_6:
            for (int i = 0; i < blockcount + 1; i++)
            {
                if (block[i].select)
                {
                    block[i].color = c6;
                }
            }
            break;
        case ID_BORDER_ON:
            border = true;
            break;
        case ID_BORDER_OFF:
            border = false;
            break;
        case ID_INFORM_ON:
            inform = true;
            break;
        case ID_INFORM_OFF:
            inform = false;
            break;
        case ID_MOVE_ON:
            MoveMode = true;
            break;
        case ID_MOVE_OFF:
            MoveMode = false;
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        DeleteObject(select);
        DeleteObject(basic);
        DeleteObject(dot);
        DeleteObject(c1);
        DeleteObject(c2);
        DeleteObject(c3);
        DeleteObject(c4);
        DeleteObject(c5);
        DeleteObject(c6);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}