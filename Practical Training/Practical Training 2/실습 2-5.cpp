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
    RECT rect;
    static TCHAR str[300];

    static int INDEX{ 0 };
    static int LineCount{ 0 };


    switch (iMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 2, 15);
        
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);

        ShowCaret(hWnd);
        if (wParam == VK_BACK) {
            // 백스페이스 처리
            HideCaret(hWnd);
            if (INDEX == 0)
            {
                LineCount--;
            }
            INDEX--;
            str[INDEX] = '\0';
        }
        
        else if (wParam == VK_ESCAPE)
        {
            HideCaret(hWnd);
            DestroyCaret();
            PostQuitMessage(0);
        }
        else if (wParam == VK_SPACE) {
            str[INDEX++] = wParam;
            str[INDEX] = '\0';

        }
        else if (wParam == VK_RETURN)
        {

            HideCaret(hWnd);
            LineCount++;
            ZeroMemory(str, lstrlen(str));
            INDEX = 0;
        if (LineCount == 10)
        {
            HideCaret(hWnd);
            LineCount = 0;
            ZeroMemory(str, lstrlen(str));
            INDEX = 0;
        }

        }
        else if (INDEX == 30) {
            HideCaret(hWnd);
            INDEX = 0;

            LineCount++;
            ZeroMemory(str, sizeof(str)); // 문자열 초기화
            if (LineCount == 10)
            {
                HideCaret(hWnd);
                LineCount = 0;
                ZeroMemory(str, lstrlen(str));
                INDEX = 0;
            }
        }
        else
        {
            str[INDEX++] = wParam;
            str[INDEX] = '\0';
        }
        InvalidateRect(hWnd, NULL, false);
        ReleaseDC(hWnd, hDC);
       
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        GetTextExtentPoint32(hDC, str, lstrlen(str), &size);
        TextOut(hDC, 0, LineCount * 15, str, lstrlen(str));
        SetCaretPos(size.cx, LineCount * 15);
        ShowCaret(hWnd);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret
        (hWnd); //--- 캐럿 숨기기
        DestroyCaret(); //--- 캐럿 삭제하기
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}