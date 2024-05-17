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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 900, 300, 518, 540, NULL, NULL, hInstance, NULL);
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
    int value;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    TCHAR Clear1[100];
    TCHAR Clear2[100];
    TCHAR Clear3[100];
    srand(time(NULL));
    int rndnum1 = rand() % 10;
    int rndnum2 = rand() % 10;
    int rndnum3 = rand() % 10;
    int rndnum4 = rand() % 10;
    int rndnum5 = rand() % 10;
    int rndnum6 = rand() % 10;
    int rndnum7 = rand() % 10;
    int rndnum8 = rand() % 10;
    int rndnum9 = rand() % 10;
    int rndnum10 = rand() % 10;

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
    int rndx7 = rand() % 19;
    int rndy7 = rand() % 16 + 2;
    int rndx8 = rand() % 19;
    int rndy8 = rand() % 16 + 2;
    int rndx9 = rand() % 19;
    int rndy9 = rand() % 16 + 2;
    int rndx10 = rand() % 19;
    int rndy10 = rand() % 16 + 2;

    static bool check[10];
    static figure player{ 10,19,1 };
    static int CurrentWord = 0;;
    static figure word[3][10]{ { {rndx3,rndy3,5,rndnum1}, {rndx2,rndy2,5,rndnum2},{rndx4,rndy4,5,rndnum3},{rndx,rndy,5,rndnum4},{rndx5,rndy5,5,rndnum5}},
        {{rndx,rndy,5,'I' - '@'}, {rndx3,rndy3,5,'N' - '@'},{rndx2,rndy2,5,'S' - '@'},{rndx4,rndy4,5,'T' - '@'},{rndx6,rndy6,5,'A' - '@'}},
       
        {{rndx8,rndy8,10,'M' - '@'}, {rndx9,rndy9,10,'O' - '@'},{rndx10,rndy10,10,'U' - '@'},{rndx6,rndy6,10,'S' - '@'},{rndx7,rndy7,10,'E'-'@'},
        {rndx3,rndy3,10,rndnum6}, {rndx2,rndy2,10,rndnum7},{rndx4,rndy4,10,rndnum8},{rndx,rndy,10,rndnum9},{rndx5,rndy5,10,rndnum10}} };
    static figure goal[5];
    static SIZE size;
    static TCHAR winningmessage[100] = L"단어를 완성했습니다!";
    static TCHAR losingmessage[100] = L"그것도 못하냐, 다시 해봐";
    static int linesize = 25;
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
        }
        else if (wParam == '1') {
            CurrentWord = 0;
        }
        else if (wParam == '2') {
            CurrentWord = 1;
        }
        else if (wParam == '3') {
            CurrentWord = 2;
        }
        else if (wParam == 'p' || wParam == 'P')
        {
            word[0][0].value = rndnum1, word[0][1].value = rndnum2, word[0][2].value = rndnum3, word[0][3].value = rndnum4, word[0][4].value = rndnum5;
            word[1][0].value = 'I' - '@', word[1][1].value = 'N' - '@', word[1][2].value = 'S' - '@', word[1][3].value = 'T' - '@', word[1][4].value = 'A' - '@';
            word[2][0].value = 'A' - '@', word[2][1].value = 'O' - '@', word[2][2].value = 'U' - '@', word[2][3].value = 'S' - '@', word[2][4].value = 'E' - '@';
            word[2][6].value = rndnum6, word[2][7].value = rndnum7, word[2][8].value = rndnum8, word[2][9].value = rndnum9, word[2][9].value = rndnum10;
            player.x = 10; // 플레이어 위치 초기화
            player.y = 19;
            // 단어 위치 초기화
            for (int i = 0; i < 10; ++i) {
                word[0][i].x = rand() % 19;
                word[0][i].y = rand() % 16 + 2;
                word[1][i].x = rand() % 19;
                word[1][i].y = rand() % 16 + 2;
                word[2][i].x = rand() % 19;
                word[2][i].y = rand() % 16 + 2;
                word[0][i].size = 5;
                word[1][i].size = 5;
                word[2][i].size = 10;

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
                if (word[CurrentWord][i].x > 0 && word[CurrentWord][i].x < 19) // 밀기
                {
                    if (player.x - 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x--;
                    }
                }
                else if (word[CurrentWord][i].x == 0) // 맵 반대편으로 밀기
                {
                    if (player.x - 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x = 19;
                    }
                }
                else if (word[CurrentWord][i].x == 19 && player.x == 0) // 맵 반대편에서 밀기
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
                if (word[CurrentWord][i].x > 0 && word[CurrentWord][i].x < 19) // 밀기
                {
                    if (player.x + 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x++;
                    }
                }
                else if (word[CurrentWord][i].x == 19) // 맵 반대편으로 밀기
                {
                    if (player.x + 1 == word[CurrentWord][i].x && player.y == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].x = 0;
                    }
                }
                else if (word[CurrentWord][i].x == 0 && player.x == 19) //맵 반대편에서 밀기
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
                if (word[CurrentWord][i].y > 0 && word[CurrentWord][i].y < 19) // 밀기
                {
                    if (player.x == word[CurrentWord][i].x && player.y - 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y--;
                    }
                }
                else if (word[CurrentWord][i].y == 0) //맵 반대편으로 밀기
                {
                    if (player.x == word[CurrentWord][i].x && player.y - 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y = 19;
                    }
                }
                else if (word[CurrentWord][i].y == 19 && player.y == 0) // 맵 반대편에서 밀기
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
            for (int i = 0; i < word[CurrentWord][0].size; i++) // 밀기
            {
                if (word[CurrentWord][i].y < 19 && word[CurrentWord][i].y > 0)
                {
                    if (player.x == word[CurrentWord][i].x && player.y + 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y++;
                    }
                }
                else if (word[CurrentWord][i].y == 19) //맵 반대편으로 밀기
                {
                    if (player.x == word[CurrentWord][i].x && player.y + 1 == word[CurrentWord][i].y)
                    {
                        word[CurrentWord][i].y = 0;
                    }
                }
                else if (word[CurrentWord][i].y == 0 && player.y == 19) //맵 반대편에서 밀기
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

        for (int i = 0; i < word[CurrentWord][0].size; i++) {  // 곂칠때 처리인데 두자리수에 대한 처리가 안됌
            for (int j = i + 1; j < word[CurrentWord][0].size; j++)
            {
                if (CurrentWord == 0)
                {
                    if (word[CurrentWord][i].x == word[CurrentWord][j].x && word[CurrentWord][i].y == word[CurrentWord][j].y)
                    {
                        word[CurrentWord][i].value = word[CurrentWord][i].value + word[CurrentWord][j].value;
                        for (int t = j; t < word[CurrentWord][0].size; t++)
                        {
                            word[CurrentWord][t] = word[CurrentWord][t + 1];
                        }
                        word[CurrentWord][0].size--;
                    }
                }
                else {
                    if (word[CurrentWord][i].x == word[CurrentWord][j].x && word[CurrentWord][i].y == word[CurrentWord][j].y)
                    {
                        if ((word[CurrentWord][i].value + word[CurrentWord][j].value) == 26)
                        {
                            word[CurrentWord][i].value = (word[CurrentWord][i].value + word[CurrentWord][j].value) % 26 + 1;
                        }
                        else {
                            word[CurrentWord][i].value = (word[CurrentWord][i].value + word[CurrentWord][j].value) % 26;

                        }
                        for (int t = j; t < word[CurrentWord][0].size; t++)
                        {
                            word[CurrentWord][t] = word[CurrentWord][t + 1];
                        }
                        word[CurrentWord][0].size--;
                    }
                }
            }   
        }
        for (int i = 0; i < word[CurrentWord][0].size; i++) {
            if (CurrentWord == 0) // 숫자
            {
                wsprintf(Clear3, L"%d", word[CurrentWord][i].value);
                TextOut(hDC, word[CurrentWord][i].x* linesize + 8, word[CurrentWord][i].y* linesize + 5, Clear3, lstrlen(Clear3));
            }
            else if (CurrentWord == 1) //알파벳
            {
                wsprintf(Clear2, L"%c", word[CurrentWord][i].value + 64);
                TextOut(hDC, word[CurrentWord][i].x* linesize + 8, word[CurrentWord][i].y* linesize + 5, Clear2, lstrlen(Clear2));
            }
            else if (CurrentWord == 2) // 알파벳 + 숫자(해결필요)
            {
                if (i<5)
                {
                    wsprintf(Clear1, L"%c", word[CurrentWord][i].value + 64);
                    TextOut(hDC, word[CurrentWord][i].x* linesize + 8, word[CurrentWord][i].y* linesize + 5, Clear1, lstrlen(Clear1));
                }
                else {
                    wsprintf(Clear1, L"%c", word[CurrentWord][i].value+'0');
                    TextOut(hDC, word[CurrentWord][i].x* linesize + 8, word[CurrentWord][i].y* linesize + 5, Clear1, lstrlen(Clear1));
                }
            }
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