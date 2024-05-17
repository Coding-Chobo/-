#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Pushing word";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 900, 300, 450, 500, NULL, NULL, hInstance, NULL);
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
    char alpha;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    TCHAR Clear1[100];
    TCHAR Clear2[100];
    TCHAR Clear3[100];
    srand(time(NULL));
    int rndx = rand() % 19;
    int rndy = rand() % 16 + 2;
    int rndx2 = rand() % 19;
    int rndy2 = rand() % 16 + 2;
    int rndx3 = rand() % 19;
    int rndy3 = rand() % 16 + 2;
    int rndx4 = rand() % 19;
    int rndy4 = rand() % 16 + 2;
    int rndx5 = rand() % 19;
    int rndy5 = rand() % 16 + 2;
    int rndx6 = rand() % 19;
    int rndy6 = rand() % 16 + 2;
    static bool check[5];
    static figure player{ 10,19,1 };
    static int CurrentWord = 0;
    static figure word[3][5]{{ {8,1,5,'A'}, {rndx2,rndy2,5,'P'},{10,1,5,'P'},{rndx,rndy,5,'L'},{12,1,5,'E'} },
        {{8,1,5,'I'}, {rndx3,rndy3,5,'N'},{10,1,5,'S'},{rndx4,rndy4,5,'T'},{12,1,5,'A'}},
        {{8,1,5,'M'}, {rndx5,rndy5,5,'O'},{10,1,5,'U'},{rndx6,rndy6,5,'S'},{12,1,5,'E'}} };
    static figure goal[5]{ {8,1}, {9,1,},{10,1},{11,1},{12,1} };
    static SIZE size;
    static TCHAR winningmessage[100] = L"단어를 완성했습니다!";
    static TCHAR losingmessage[100] = L"그것도 못하냐, 다시 해봐";
    static int linesize = 20;
    static int mapsize = 20;
    switch (iMsg)
    {
    case WM_CREATE:
        
        static HBRUSH playercolor = CreateSolidBrush(RGB(255, 0, 0));

        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        if (wParam == 'q' || wParam == 'Q') {
            PostQuitMessage(0);
            DeleteObject(playercolor);
        }else if (wParam == 's' || wParam == 'S') {
            if (CurrentWord == 2)
            {
                CurrentWord = 0;
            }
            else {
                CurrentWord++;
            }
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        if (wParam == VK_LEFT) {

            for (int i = 0; i < word[CurrentWord][0].size; i++)
            {
                if (word[CurrentWord][i].x > 0 && word[CurrentWord][i].x < 19)
                {
                    if (player.x - 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y) 
                    {
                        word[CurrentWord][i].x--;
                    }
                }
                else if (word[CurrentWord][i].x == 0)
                {
                    if (player.x - 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x = 19;
                    }
                }
                else if (word[CurrentWord][i].x == 19 && player.x == 0)
                {
                    if (player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x--;
                    }
                }
            }
            if (player.x == 0)
            {
                player.x = 19;
            }
            else
            {
                player.x--;
            }
        }
        else  if (wParam == VK_RIGHT) {
            for (int i = 0; i < word[CurrentWord][0].size; i++)
            {
                if (word[CurrentWord][i].x >0 &&word[CurrentWord][i].x < 19)
                {
                    if (player.x + 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x++;
                    }
                }
                else if (word[CurrentWord][i].x == 19)
                {
                    if (player.x+1 == word[CurrentWord][i].x && player.y== word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x = 0;
                    }
                }
                else if (word[CurrentWord][i].x == 0 && player.x == 19)
                {
                    if (player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x++;
                    }
                }
            }
            if (player.x == 19)
            {
                player.x = 0;
            }
            else
            {
                player.x++;
            }
        }
        else if (wParam == VK_UP) {
            for (int i = 0; i < word[CurrentWord][0].size; i++)
            {
                if (word[CurrentWord][i].y > 0&& word[CurrentWord][i].y <19)
                {
                    if (player.x == word[CurrentWord][i].x && player.y -1 == word[CurrentWord][i].y) 
                    {
                        word[CurrentWord][i].y--;
                    }
                }
                else if (word[CurrentWord][i].y == 0)
                {
                    if (player.x == word[CurrentWord][i].x && player.y -1 == word[CurrentWord][i].y) 
                    {
                        word[CurrentWord][i].y =19;
                    }
                }
                else if (word[CurrentWord][i].y == 19 && player.y == 0)
                {
                    if (player.x == word[CurrentWord][i].x)
                    {
                        word[CurrentWord][i].y--;
                    }
                }
            }
            if (player.y == 0)
            {
                player.y = 19;
            }
            else
            {
                player.y--;
            }
        }
        else if (wParam == VK_DOWN) {
            for (int i = 0; i < word[CurrentWord][0].size; i++)
            {
                if (word[CurrentWord][i].y < 19 && word[CurrentWord][i].y > 0)
                {
                    if (player.x == word[CurrentWord][i].x && player.y + 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y++;
                    }
                }
                else if (word[CurrentWord][i].y == 19)
                {
                    if (player.x == word[CurrentWord][i].x && player.y + 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y = 0;
                    }
                }
                else if (word[CurrentWord][i].y == 0 && player.y == 19)
                {
                    if (player.x == word[CurrentWord][i].x)
                    {
                        word[CurrentWord][i].y++;
                    }
                }
            }
            if (player.y == 19)
            {
                player.y = 0;
            }
            else
            {
                player.y++;
            }
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        for (int i = 0; i <= mapsize; i++) // 맵 구현
        {
            MoveToEx(hDC, i * linesize, 0, NULL);
            LineTo(hDC, i * linesize, mapsize * linesize);
            MoveToEx(hDC, 0, i * linesize, NULL);
            LineTo(hDC, mapsize * linesize, i * linesize);
        }
        for (int i = 0; i < word[CurrentWord][0].size; i++) {
            TCHAR str[2] = { word[CurrentWord][i].alpha, '\0' };
            TextOut(hDC, word[CurrentWord][i].x * linesize+5, word[CurrentWord][i].y * linesize+2, str, 1);
        }
        for (int i = 0; i < word[CurrentWord][0].size; i++) {
                if ((word[CurrentWord][i].x == goal[i].x)&&(word[CurrentWord][i].y==goal[i].y))
                {
                    check[i] = true;
                }
                else
                {
                    check[i] = false;
                }
        }
        if ((check[0] == true)&& (check[1] == true)&& (check[2] == true)&& (check[3] == true)&& (check[4] == true))
        {
            TextOut(hDC, 100, 430, winningmessage, lstrlen(winningmessage));
        }

        SelectObject(hDC, playercolor);
        Ellipse(hDC, player.x * linesize + 2 * player.size, player.y * linesize + 2 * player.size, (player.x + 1) * linesize - 2 * player.size, (player.y + 1) * linesize - 2 * player.size);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        DeleteObject(playercolor);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}