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

    static int rnd;
    static int x{ 800 / dx };
    static int y{ 600 / dy };
    static SIZE size;
    static TCHAR str[100];
    static TCHAR result[100];
    static int INDEX{ 0 };
    static int LineCount{ 0 };


    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));

        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 1; i < dx; i++){
            for (int j = 0; j < 70; j++)
            {
                TextOut(hDC, i * x,j*12,L"|",1);
            }
        }
        for (int i = 1; i < dy; i++) {
            for (int j = 0; j < 200; j++)
            {
                TextOut(hDC, j*4, i * y , L"-", 1);
            }
        }
        for (int i = 0; i < dx; i++) {
            for (int j = 0; j < dy; j++)
            {
                rnd = rand() % 5;
                rect.left = i * x;
                rect.top = j * y;
                rect.right = (i + 1) * x;
                rect.bottom = (j + 1) * y;
                int xmiddle = (rect.left + rect.right) / 2;
                int ymiddle = (rect.top + rect.bottom) / 2;
                int count{ 0 };
                if (rnd==0)//x그림 그리기
                {
                    for (int t = 0; (ymiddle + 5 * t < rect.bottom - 5) && (xmiddle + 5 * t < rect.right - 5); t++)
                    {
                        COLORREF rndtextclr = RGB(255, 0, 0);
                        COLORREF rndbkclr = RGB(0, 255, 0);
                        SetTextColor(hDC, rndtextclr);
                        SetBkColor(hDC, rndbkclr);
                        TextOut(hDC, xmiddle + 5 * t, ymiddle + 5 * t, L"x", 1);
                        TextOut(hDC, xmiddle - 5 * t, ymiddle + 5 * t, L"x", 1);
                        TextOut(hDC, xmiddle + 5 * t, ymiddle - 5 * t, L"x", 1);
                        TextOut(hDC, xmiddle - 5 * t, ymiddle - 5 * t, L"x", 1);
                    }
                }if (rnd == 1) {//삼각형 두개 만들기
                    for (int k = 0; (ymiddle + 5 * k <= rect.bottom - 5) && (xmiddle + 5 * k <= rect.right - 5); k++)
                    {
                        COLORREF rndtextclr = RGB(100, 100, 0);
                        COLORREF rndbkclr = RGB(0, 40, 200);
                        SetTextColor(hDC, rndtextclr);
                        SetBkColor(hDC, rndbkclr);
                        TextOut(hDC, xmiddle + 5 * k, ymiddle + 5 * k, L"V", 1);
                        TextOut(hDC, xmiddle - 5 * k, ymiddle + 5 * k, L"v", 1);
                        TextOut(hDC, xmiddle + 5 * k, rect.top + 5 * k, L"v", 1);
                        TextOut(hDC, xmiddle - 5 * k, rect.top + 5 * k, L"v", 1);
                        count++;
                    }
                    for (int wer = 0; (xmiddle + 5 * wer <= xmiddle + 5 * count); wer++)
                    {
                        TextOut(hDC, xmiddle - 5 * wer, rect.top + 5 * count, L"V", 1);
                        TextOut(hDC, xmiddle + 5 * wer, rect.top + 5 * count, L"V", 1);
                        TextOut(hDC, xmiddle - 5 * wer, ymiddle + 5 * count, L"V", 1);
                        TextOut(hDC, xmiddle + 5 * wer, ymiddle + 5 * count, L"V", 1);
                    }
                }if (rnd == 2) {//ㄹ그림 그리기
                    for (int k = 0; (ymiddle + 5 * k < rect.bottom-5) && (xmiddle + 5 * k < rect.right-5); k++)
                    {
                        COLORREF rndtextclr = RGB(255, 255, 255);
                        COLORREF rndbkclr = RGB(0, 0, 0);
                        SetTextColor(hDC, rndtextclr);
                        SetBkColor(hDC, rndbkclr);
                            TextOut(hDC, xmiddle - 5 * k, rect.top + 5, L"~", 1);
                            TextOut(hDC, xmiddle - 5 * k, ymiddle, L"~", 1);
                            TextOut(hDC, xmiddle - 5 * k, rect.bottom - 5, L"~", 1);
                            TextOut(hDC, xmiddle + 5 * k, rect.top + 5, L"~", 1);
                            TextOut(hDC, xmiddle + 5 * k, ymiddle, L"~", 1);
                            TextOut(hDC, xmiddle + 5 * k, rect.bottom - 5, L"~", 1);
                            TextOut(hDC, xmiddle + 5 * k, rect.bottom - 5, L"~", 1);
                            count++;
                    }
                    for (int r = 1; (ymiddle + 5 * r <= rect.bottom); r++)
                    {
                        TextOut(hDC, xmiddle - 5 * count, rect.bottom - 5 * r, L"~", 1);
                        TextOut(hDC, xmiddle + 5 * count, rect.top + 5 * r, L"~", 1);
                    }
                }if (rnd == 3) {//마름모 그림 그리기
                    for (int k = 0; ((rect.bottom + ymiddle) / 2 - 5 * k > ymiddle) && (xmiddle + 5 * k < rect.right - 5); k++)
                    {
                        COLORREF rndtextclr = RGB(0, 0, 5);
                        COLORREF rndbkclr = RGB(100, 20, 100);
                        SetTextColor(hDC, rndtextclr);
                        SetBkColor(hDC, rndbkclr);
                        TextOut(hDC, xmiddle + 5 * k, (rect.bottom + ymiddle) / 2 - 5 * k, L"o", 1);
                        TextOut(hDC, xmiddle - 5 * k, (rect.bottom + ymiddle) / 2 - 5 * k, L"o", 1);
                        TextOut(hDC, xmiddle + 5 * k, (rect.top+ymiddle)/2 + 5 * k, L"o", 1);
                        TextOut(hDC, xmiddle - 5 * k, (rect.top + ymiddle) / 2 + 5 * k, L"o", 1);

                    }
                }if (rnd == 4) {//나비 그림 그리기
                    for (int t = 0; (ymiddle + 5 * t < rect.bottom - 5) && (xmiddle + 5 * t < rect.right - 5); t++)
                    {
                        COLORREF rndtextclr = RGB(255, 0, 255);
                        COLORREF rndbkclr = RGB(122, 122, 122);
                        SetTextColor(hDC, rndtextclr);
                        SetBkColor(hDC, rndbkclr);
                        TextOut(hDC, xmiddle + 5 * t, ymiddle + 5 * t, L"*", 1);
                        TextOut(hDC, xmiddle - 5 * t, ymiddle + 5 * t, L"*", 1);
                        TextOut(hDC, xmiddle + 5 * t, ymiddle - 5 * t, L"*", 1);
                        TextOut(hDC, xmiddle - 5 * t, ymiddle - 5 * t, L"*", 1);
                        count++;
                    }
                    for (int r = 1; (ymiddle + 5 * r < ymiddle + 5 * count); r++)
                    {
                        TextOut(hDC, xmiddle - 5 * count, ymiddle - 5 * r, L"*", 1);
                        TextOut(hDC, xmiddle - 5 * count, ymiddle + 5 * r, L"*", 1);
                        TextOut(hDC, xmiddle + 5 * count, ymiddle - 5 * r, L"*", 1);
                        TextOut(hDC, xmiddle + 5 * count, ymiddle + 5 * r, L"*", 1);
                    }
                }
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
