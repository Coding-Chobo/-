#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"windows program2";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    
    
    int x = rand() % 600 + 1;
    int y = rand() % 400 + 1;
    int n = rand() % 9;
    int count = rand() % 16 + 5;
    HDC hDC;
    PAINTSTRUCT ps;
    TCHAR num[25];
    for (int i = 0; i < count; i++)
    {
        num[i] = n;
    }
    COLORREF rndtextclr = RGB(rand() % 256, rand() % 256, rand() % 256);
    COLORREF rndbkclr = RGB(rand() % 256, rand() % 256, rand() % 256);

    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        SetTextColor(hDC, rndtextclr);
        SetBkColor(hDC, rndbkclr);

        wsprintf(num, L"%d", n);
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count; j++) {
                TextOut(hDC, x + (i * 8), y + (j * 13), num, 1);
            }
        }

        EndPaint(hWnd, &ps);
        break; ;
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {

            x = rand() % 600 + 1;
            y = rand() % 400 + 1;
            n = rand() % 9;
            count = rand() % 16 + 5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        if (wParam == 'Q')
        {
        PostQuitMessage(0);
        break;
        }
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}   