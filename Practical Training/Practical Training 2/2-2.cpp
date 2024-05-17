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
    srand(time(NULL));
    int dx = rand() % 9 + 2;
    int dy = rand() % 9 + 2;
    HDC hDC;
    PAINTSTRUCT ps;
    RECT rect;
    int x = 800 / dx;
    int y = 600 / dy;

    switch (iMsg)
    {
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = -0; i < dx; i++){
            for (int j = 0; j < dy; j++)
            {
                rect.left = i * x;
                rect.top = j * y;
                rect.right = (i + 1) * x;
                rect.bottom = (j + 1) * y;
                COLORREF rndtextclr = RGB(rand() % 256, rand() % 256, rand() % 256);
                COLORREF rndbkclr = RGB(rand() % 256, rand() % 256, rand() % 256);
                SetTextColor(hDC, rndtextclr);
                SetBkColor(hDC, rndbkclr);
                DrawText(hDC, L"ABCDEFGHIJHLMNOPQRSXYZ", -1, &rect, DT_WORDBREAK | DT_EDITCONTROL);
            }
        }
        
        EndPaint(hWnd, &ps);
        break; ;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}