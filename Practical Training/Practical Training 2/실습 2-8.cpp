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
    static TCHAR temp[10][30];
    static int INDEX{ 0 };
    static int LineCount{ 0 };
    static bool a{ true };
    static bool b{ true };
    static bool c{ true };
    static bool d{ true };
    static bool e{ true };
    int count = 0;   
    switch (iMsg)
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
        else if (wParam == '+') //+키 구현
        {
            for (int i = 0; i < 10; i++){
                for (int j = 0; j < lstrlen(str[i]); j++)
                {
                    if (str[i][j] >= 'A' && str[i][j] < 'z')
                    {
                        str[i][j]++;
                    }
                    else if (str[i][j]=='z')
                    {
                        str[i][j] = 'A';
                    }
                    else if (str[i][j] >= '0' && str[i][j] < '9')
                    {
                        str[i][j]++;
                    }
                    else if (str[i][j]=='9')
                    {
                        str[i][j] = '0';
                    }
                }
            }
            InvalidateRect(hWnd, NULL, true);
        }
        else if (wParam == '-') //-키 구현
        {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < lstrlen(str[i]); j++)
                {
                    if (str[i][j] > 'A' && str[i][j] <= 'z')
                    {
                        str[i][j]--;
                    }
                    else if (str[i][j] == 'A')
                    {
                        str[i][j] = 'z';
                    }
                    else if (str[i][j] > '0' && str[i][j] <= '9')
                    {
                        str[i][j]--;
                    }
                    else if (str[i][j] == '0')
                    {
                        str[i][j] = '9';
                    }
                }
            }
            InvalidateRect(hWnd, NULL, true);
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
            if ((wParam >= 'a' && wParam <= 'z') && (a == false)) {
                wParam = toupper(wParam);
                str[LineCount][INDEX++] = wParam;
                str[LineCount][INDEX] = '\0';
            }
            else if (b == false)
            {
                for (int i = 0; i < lstrlen(str[LineCount]) - INDEX; i++)
                {
                    str[LineCount][lstrlen(str[LineCount]) - i] = str[LineCount][lstrlen(str[LineCount]) - 1 - i];
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
            for (int i = wordStart; str[LineCount][i] != '\0'; ++i)
            {
                str[LineCount][i] = str[LineCount][i + (INDEX - wordStart)];
            }
            INDEX = wordStart;
            InvalidateRect(hWnd, NULL, TRUE);
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
        else if (wParam == VK_F2) {
            if (c) {
                for (int j = 0; j < 10; j++) {
                    for (int i = lstrlen(str[j]); i >= 0; i--) {
                        str[j][i + 4] = str[j][i];
                    }
                    for (int i = 0; i < 4; i++) {
                        str[j][i] = '_';
                    }
                }
                INDEX += 4;
                c = false;
            }
            else {
                for (int j = 0; j < 10; j++) {
                    for (int i = 0; i <= lstrlen(str[j]) - 4; i++) {
                        str[j][i] = str[j][i + 4];
                    }
                }
                INDEX -= 4;
                c = true;
            }
            InvalidateRect(hWnd, NULL,true);
        }
        else if (wParam == VK_F3){
            for (int i = 8; i >=0; i--) {
                for (int j = 0; j < 30; j++) {
                    str[i + 1][j] = str[i][j];
                }
            }if (LineCount < 9)
            {
                LineCount++;
            }
            INDEX = lstrlen(str[LineCount]);
            memset(str[0], 0, sizeof(str[0]));
            InvalidateRect(hWnd, NULL, true);
        }
        else if (wParam == VK_F4) {
            char temp[30];
            for (int i = 0; i < 5 ; i++) {
                for (int j = 0; j <= max(lstrlen(str[i]),lstrlen(str[9-i])); j++) {
                    temp[j] = str[i][j];
                    str[i][j] = str[9 - i][j];
                    str[9 - i][j] = temp[j];
                }
            }
            
            INDEX = lstrlen(str[LineCount]);
            InvalidateRect(hWnd, NULL, true);
            }
        else if (wParam == VK_PRIOR) { 
            if (e) {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 30; j++) {
                        temp[i][j] = str[i][j];
                    }
                }
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < lstrlen(str[i]); j++) {
                        if (str[i][j] == ' ') {
                            for (int t = j; t < lstrlen(str[i]); t++)
                            {
                                str[i][t] = str[i][t + 1]; 
                            }
                        }
                    }
                }
                INDEX = lstrlen(str[LineCount]);
                e = false;
            }
            else {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 30; j++) {
                        str[i][j] = temp[i][j];
                    }
                }
                INDEX = lstrlen(str[LineCount]);
                e = true;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_NEXT) {
            if (d) {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 30; j++) {
                        temp[i][j] = str[i][j];
                    }
                }


                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < lstrlen(str[i]); j++) {
                        if (str[i][j] == ' '&& lstrlen(str[i]) + 2 < 30) {
                            for (int t = lstrlen(str[i]); t > j; t--)
                            {
                                str[i][t + 1] = str[i][t];
                            }
                            str[i][j] = ')';
                            str[i][j + 1] = '(';
                        }
                    }
                }
                    
                for (int i = 0; i < 10; i++) {
                    int len = lstrlen(str[i]);
                    if (len > 0 && len + 1 < 30) {
                        for (int k = len - 1; k >= 0; k--) {
                            str[i][k + 1] = toupper(str[i][k]);
                        }
                        str[i][0] = '(';
                        str[i][len + 1] = ')'; 
                        str[i][len + 2] = '\0';
                    }
                }
                INDEX = lstrlen(str[LineCount]);
                d = false;
            }
            else {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 30; j++) {
                        str[i][j] = temp[i][j];
                    }       
                }
                INDEX = lstrlen(str[LineCount]);
                d = true;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 0; i < 10; ++i)
        {
            TextOut(hDC, 0, i * 20, str[i], min(30, lstrlen(str[i])));
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
