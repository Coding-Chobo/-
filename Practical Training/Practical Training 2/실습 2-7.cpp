#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 2-7";
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
    HDC hDC;
    PAINTSTRUCT ps;
    static SIZE size;
    RECT rect;
    static TCHAR str[10][30];

    static int INDEX{ 0 };
    static int LineCount{ 0 };
    static bool a{ true };
    static bool b{ true };
    int count = 0;    switch (iMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 2, 15);
        INDEX = 0;
        LineCount = 0;
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        HideCaret(hWnd);
        if (wParam == VK_BACK) {// 백스페이스 처리
            if (INDEX == 0 && LineCount > 0)
            {
                LineCount--;
                INDEX = lstrlen(str[LineCount]) + 1;
            }
            else if (INDEX != 0)
            {
                INDEX--;
                str[LineCount][INDEX] = '\0';
            }
        }
        else if (wParam == VK_ESCAPE) //ESC키 구현
        {
            InvalidateRect(hWnd, NULL, true);
            memset(str, 0, sizeof(str));
            INDEX = 0;
            LineCount = 0;
        }
        else if (wParam == VK_TAB) //TAP키 구현
        {
            for (int i = 0; i < 5; i++)
            {
                str[LineCount][INDEX++] = VK_SPACE;
                str[LineCount][INDEX] = '\0';
            }
        }
        else if (wParam == VK_SPACE)//스페이스바 키 구현
        {
            str[LineCount][INDEX++] = wParam;
            str[LineCount][INDEX] = '\0';
        }
        else if (wParam == VK_RETURN) // ENTER키 구현
        {
            if (LineCount != 9)
            {
                LineCount++;
                INDEX = 0;
            }
            else if (LineCount == 9)
            {
                LineCount = 0;
                INDEX = 0;
            }
        }   
        else if (lstrlen(str[LineCount]) == 30) {
            if (LineCount != 9)
            {
                LineCount++;
                INDEX = 0;
            }
            else if (LineCount == 9)
            {
                LineCount = 0;
                INDEX = 0;
            }
        }
        else {
            if ((wParam >= 'a' && wParam <= 'z')&& (a == false)) {
                wParam = toupper(wParam);
                str[LineCount][INDEX++] = wParam;
                str[LineCount][INDEX] = '\0';
            }else if (b == false)
            {
                for (int i = 0; i < lstrlen(str[LineCount])-INDEX; i++)
                {
                    str[LineCount][lstrlen(str[LineCount])- i] = str[LineCount][lstrlen(str[LineCount]) -1- i];
                }
                str[LineCount][INDEX] = wParam;
            }
            else
            {
                str[LineCount][INDEX++] = wParam;
                str[LineCount][INDEX] = '\0';
            }
        }
        

        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, false);
        ShowCaret(hWnd);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        HideCaret(hWnd);
        if (wParam == VK_LEFT)//왼쪽으로 이동
        {
            if (INDEX > 0)
            {
                INDEX--;
            }
            else if (INDEX == 0 && LineCount > 0)
            {
                LineCount--;
                INDEX = lstrlen(str[LineCount]);

            }
        }
        else if (wParam == VK_RIGHT)//오른쪽으로 이동
        {
            if (INDEX < lstrlen(str[LineCount]) + 1)
            {
                INDEX++;
            }
            else
            {
                str[LineCount][INDEX++] = ' ';
                str[LineCount][INDEX] = '\0';
            }
        }
        else if (wParam == VK_UP)
        {
            if (LineCount > 0)
            {
                LineCount--;
                INDEX = min(INDEX, lstrlen(str[LineCount]));

            }
        }
        else if (wParam == VK_DOWN)
        {
            if (LineCount < 9)
            {

                LineCount++;
                INDEX = min(INDEX, lstrlen(str[LineCount]));
            }
        }
        else if (wParam == VK_F1)
        {
            if (a == true)
            {
                a = false;
            }
            else if (a == false)
            {
                a = true;
            }
        }
        else if (wParam == VK_DELETE)
        {
            int wordStart = INDEX;
            while (wordStart > 0 && str[LineCount][wordStart - 1] != ' ')
            {
                wordStart--;
            }
            // 해당 단어 삭제 및 문자열 재정렬
            for (int i = wordStart; str[LineCount][i] != '\0'; ++i)
            {
                str[LineCount][i] = str[LineCount][i + (INDEX - wordStart)];
            }
            // 커서 위치 업데이트
            INDEX = wordStart;
        }
        else if (wParam == VK_INSERT)
        {
            if (b)
            {
                b = false;
            }
            else if (b == false)
            {
                b = true;
            }
        }
        else if (wParam == VK_HOME) { // Home키 구현
            INDEX = 0;
        }
        else if (wParam == VK_END) { // End키 구현
            INDEX = lstrlen(str[LineCount]);
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        ShowCaret(hWnd);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 0; i < 10; ++i)
        {
            TextOut(hDC, 0, i * 20, str[i], lstrlen(str[i]));
        }
        GetTextExtentPoint32(hDC, str[LineCount], INDEX, &size);
        SetCaretPos(size.cx, LineCount * 20);
        ShowCaret(hWnd);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret(hWnd); 
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}



