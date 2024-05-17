#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 2-4";
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
    static int x{ 0 }, y{ 0 }, n{ 0 }, m{ 0 };
    HDC hDC;
    PAINTSTRUCT ps;
    static SIZE size;
    static TCHAR str[100];
    static TCHAR result[100];
    static int INDEX = 0;
    static int count = 0;
    static int number = 0;
    static int temp{ 0 };

    switch (iMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 2, 15);
        ShowCaret(hWnd);
        count = 0;
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        if (wParam == 'q')
        {

            HideCaret(hWnd);
            DestroyCaret();
            PostQuitMessage(0);
        }
        else if (wParam >= '0' && wParam <= '9') {
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
            switch (count++) {
            case 0: x = number; break;
            case 1: y = number; break;
            case 2: n = number; break;
            case 3: m = number; break;
            }
            INDEX = 0;
            ZeroMemory(str, sizeof(str)); // 문자열 초기화
            number = 0; // number 초기화
        }
        
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        if (count == 4)
        {
            InvalidateRect(hWnd, NULL, TRUE);

            for (int i = 1; i < m + 1; i++)
            {
                wsprintf(result, L"%d * %d = %d", n, i, n * i);
                TextOut(hDC, x, y + (15 * i), result, lstrlen(result));
            }
            count = 0;
            INDEX = 0;
            ZeroMemory(str, sizeof(str));
        }
        TextOut(hDC, temp * 10, 0, str, lstrlen(str));
        GetTextExtentPoint32(hDC, str, INDEX, &size);
        SetCaretPos(size.cx, 0);
        
        EndPaint(hWnd, &ps);
        break;

    default:
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}