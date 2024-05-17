#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <cmath>
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 1000, NULL, NULL, hInstance, NULL);
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
    HBRUSH color;
    bool Right;
    bool Left;
    bool Down;
    bool Up;
};
#define Max 20
#define BSIZE 10 //--- 반지름
#define TIMER_RESTORE_SIZE 100
#define TailsMove 50
//--- (x1, y1)과 (x2, y2)간의 길이
float LengthPts(int x1, int y1, int x2, int y2)
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
// 원과 점 충돌검사
BOOL InCircle(int x1, int y1, int x2, int y2)
{
    if (LengthPts(x1, y1, x2, y2) < BSIZE) //--- BSIZE: 반지름
        return TRUE;
    else
        return FALSE;
}
//윈도우 콜백함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC,mDC;
    HBITMAP hBitmap;

    PAINTSTRUCT ps;
    static SIZE size;
    RECT rt;
    srand(time(NULL));
    static TCHAR str[300];
    TCHAR Clear1[100];
    static figure player{ 0,0,0 };
    static figure StartingPoint;
    static figure Player_tail[Max];
    static figure MovingTail[Max];
    static figure SaveMovingTail[Max];
    static figure tail[Max];
    static figure Block[Max];
    static BOOL Selection;
    static int INDEX{ 0 }; 
    static int LineCount{ 0 };
    static int Linesize = 20;
    static int Mapsize = 40;
    static int MoveSpeed = 100;
    static int SaveSpeed = 0;
    static bool left = false;
    static bool right = false;
    static bool up = false;
    static bool down = false;
    static bool TailMove = false;
    static bool Block_OnOff = true;
    static bool jump = false;
    int mx, my, mx2, my2;
    static int prevx, prevy;
    static int tailcount{ 20 }, mtailcount{ 0 }, ptailcount{ 0 }, blockcount{ 0 }, smcount{ 0 }, sptcount, jumptime{ 0 };
    switch (iMsg)
    {
    case WM_CREATE:
        //플레이어 색 랜덤으로 뽑기
        player.color = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        StartingPoint.color = CreateSolidBrush(RGB(200, 200, 200));
        //블록 색설정
        static HBRUSH blockcolor = CreateSolidBrush(RGB(0, 0, 0));
        //먹이 정보값 설정
        for (int j = 0; j < Max; j++)
        {
            MovingTail[j].Left = false;
            MovingTail[j].Right = false;
            MovingTail[j].Down = false;
            MovingTail[j].Up = false;
        }
        for (int i = 0; i < Max; i++)
        {
            tail[i].x = rand() % (Mapsize - 1) + 1;
            tail[i].y = rand() % (Mapsize - 1) + 1;
            tail[i].color = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
            MovingTail[i].size = rand() % 4;
            switch (MovingTail[i].size)
            {
            case 0:
                MovingTail[i].Down = true;
                break;
            case 1:
                MovingTail[i].Right = true;
                break;
            case 2:
                MovingTail[i].Up = true;
                break;
            default:
                break;
            }
        }


        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        // 주인공 속도 증감
        if (wParam == '+') {
            if (MoveSpeed > 10)
            {
                MoveSpeed -= 10;
            }

            SetTimer(hWnd, 1, MoveSpeed, NULL);
        }
        else if (wParam == '-') {
            if (MoveSpeed < 990)
            {
                MoveSpeed += 10;
            }
            SetTimer(hWnd, 1, MoveSpeed, NULL);
        }
        // 게임 종료 키
        else if (wParam == 'q')
        {
            KillTimer(hWnd, 1);
            PostQuitMessage(0);
        }
        ReleaseDC(hWnd, hDC);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        //스타트 버튼
        if (wParam == 's' || wParam == 'S') {
            right = true;
            SetTimer(hWnd, 1, MoveSpeed, NULL);
        }
        //방향 키 설정
        else if (wParam == VK_UP)
        {
            left = false;
            right = false;
            down = false;
            up = true;
        }
        else if (wParam == VK_DOWN)
        {
            left = false;
            right = false;
            down = true;
            up = false;
        }
        else if (wParam == VK_RIGHT)
        {
            left = false;
            right = true;
            down = false;
            up = false;
        }
        else if (wParam == VK_LEFT)
        {
            left = true;
            right = false;
            down = false;
            up = false;
        }
        // 꼬리들 색바꾸기
        else if (wParam == 't' || wParam == 'T') {
            Player_tail[ptailcount].color = player.color;
            player.color = Player_tail[0].color;
            for (int i = 0; i < ptailcount; i++)
            {
                Player_tail[i].color = Player_tail[i + 1].color;
            }
        }
        // 점프 키
        else if (wParam == 'j' || wParam == 'J') {
            jump = true;
            jumptime = 0;
        }
        //A모드
        else if (wParam == 'a' || wParam == 'A') {
            SaveSpeed = MoveSpeed;
            MoveSpeed = 10;
            StartingPoint.x = player.x;
            StartingPoint.y = player.y;
            Block_OnOff = false;
            for (int i = 0; i < mtailcount; i++)
            {
                SaveMovingTail[i].x = MovingTail[i].x;
                SaveMovingTail[i].y = MovingTail[i].y;
                SaveMovingTail[i].color = MovingTail[i].color;
                SaveMovingTail[i].size = MovingTail[i].size;
                SaveMovingTail[i].Up = MovingTail[i].Up;
                SaveMovingTail[i].Down = MovingTail[i].Down;
                SaveMovingTail[i].Left = MovingTail[i].Left;
                SaveMovingTail[i].Right = MovingTail[i].Right;
            }
            smcount = mtailcount;
            sptcount = ptailcount;
            for (int i = 0; i < mtailcount; i++)
            {
                Player_tail[ptailcount].x = MovingTail[i].x;
                Player_tail[ptailcount].y = MovingTail[i].y;
                Player_tail[ptailcount].color = MovingTail[i].color;
                ptailcount++;
            }
            mtailcount = 0;
            SetTimer(hWnd, 1, MoveSpeed, NULL);
        }
        ReleaseDC(hWnd, hDC);
        break;
    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        //플레이어 클릭시 크기 감소
        if (InCircle(player.x * Linesize + Linesize / 2, player.y * Linesize + Linesize / 2, mx, my))
        {
            player.size = 5;
            SetTimer(hWnd, TIMER_RESTORE_SIZE, 2500, NULL);
        }
        else
        {
            //플레이어 꼬리 클릭시 분리
            for (int i = 0; i < ptailcount; i++)
            {
                if (InCircle(Player_tail[i].x * Linesize + Linesize / 2, Player_tail[i].y * Linesize + Linesize / 2, mx, my))
                {
                    for (int j = i; j < ptailcount; j++)
                    {
                        MovingTail[mtailcount].x = Player_tail[j].x;
                        MovingTail[mtailcount].y = Player_tail[j].y;
                        MovingTail[mtailcount].color = Player_tail[j].color;

                        mtailcount++;
                    }
                    ptailcount = i;
                }
            }
            //클릭으로 방향전환
            if (player.x * Linesize + Linesize / 2 < mx && player.y * Linesize + Linesize / 2 < my)
            {
                left = false;
                right = false;
                down = true;
                up = false;
            }
            else if (player.x * Linesize + Linesize / 2 > mx && player.y * Linesize + Linesize / 2 < my)
            {
                left = true;
                right = false;
                down = false;
                up = false;
            }
            else if (player.x * Linesize + Linesize / 2 < mx && player.y * Linesize + Linesize / 2 > my)
            {
                left = false;
                right = true;
                down = false;
                up = false;
            }
            else if (player.x * Linesize + Linesize / 2 > mx && player.y * Linesize + Linesize / 2 > my)
            {
                left = false;
                right = false;
                down = false;
                up = true;
            }
        }
        break;
    case WM_RBUTTONDOWN:
        mx2 = LOWORD(lParam);
        my2 = HIWORD(lParam);
        //우클릭시 블럭 생성
        if (blockcount < 20)
        {
            Block[blockcount].x = mx2 / Linesize;
            Block[blockcount].y = my2 / Linesize;
            blockcount++;
        }
        break;
    case WM_PAINT:
        GetClientRect(hWnd, &rt);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        Rectangle(mDC, 0, 0, rt.right, rt.bottom);
        for (int i = 0; i <= Mapsize; i++) // 맵 구현
        {
            MoveToEx(mDC, i * Linesize, 0, NULL);
            LineTo(mDC, i * Linesize, Mapsize * Linesize);
            MoveToEx(mDC, 0, i * Linesize, NULL);
            LineTo(mDC, Mapsize * Linesize, i * Linesize);
        }

        // 정보 출력
        wsprintf(Clear1, L"MoveSpeed : %d", MoveSpeed);
        TextOut(mDC, 40, 830, Clear1, lstrlen(Clear1));
        wsprintf(Clear1, L"장애물 의 개수: %d", blockcount);
        TextOut(mDC, 40, 850, Clear1, lstrlen(Clear1));
        wsprintf(Clear1, L"먹이의 개수 : %d", tailcount);
        TextOut(mDC, 200, 830, Clear1, lstrlen(Clear1));
        wsprintf(Clear1, L"움직이는 꼬리원의 개수 : %d", mtailcount);
        TextOut(mDC, 200, 850, Clear1, lstrlen(Clear1));
        wsprintf(Clear1, L"플레이어의 꼬리의 개수 : %d", ptailcount);
        TextOut(mDC, 200, 870, Clear1, lstrlen(Clear1));

        // 움직이는 꼬리와 플레이어 간 충돌검사
        for (int i = 0; i < mtailcount; i++)
        {
            if (player.x == MovingTail[i].x && player.y == MovingTail[i].y)
            {
                Player_tail[ptailcount].x = MovingTail[i].x;
                Player_tail[ptailcount].y = MovingTail[i].y;
                Player_tail[ptailcount].color = MovingTail[i].color;
                mtailcount--;
                ptailcount++;
                for (int j = i; j < Max - 1; j++)
                {
                    MovingTail[j].x = MovingTail[j + 1].x;
                    MovingTail[j].y = MovingTail[j + 1].y;
                    MovingTail[j].color = MovingTail[j + 1].color;
                    MovingTail[j].size = MovingTail[j + 1].size;
                    MovingTail[j].Up = MovingTail[j + 1].Up;
                    MovingTail[j].Down = MovingTail[j + 1].Down;
                    MovingTail[j].Left = MovingTail[j + 1].Left;
                    MovingTail[j].Right = MovingTail[j + 1].Right;

                }
            }
        }
        // 먹이와 플레이어 간 충돌검사
        for (int i = 0; i < tailcount; i++)
        {
            if (player.x == tail[i].x && player.y == tail[i].y)
            {
                MovingTail[mtailcount].x = tail[i].x;
                MovingTail[mtailcount].y = tail[i].y;
                MovingTail[mtailcount].color = tail[i].color;
                mtailcount++;
                tailcount--;
                for (int j = i; j < Max - 1; j++)
                {
                    tail[j].x = tail[j + 1].x;
                    tail[j].y = tail[j + 1].y;
                    tail[j].color = tail[j + 1].color;
                }
            }
        }
        if (mtailcount > 0)
        {
            TailMove = true;

            SetTimer(hWnd, TailsMove, 200, NULL);
        }
        else if (mtailcount == 0)
        {
            TailMove = false;

        }

        // 먹이 그리기
        for (int i = 0; i < tailcount; i++)
        {
            SelectObject(mDC, tail[i].color);
            Rectangle(mDC, tail[i].x * Linesize + 2, tail[i].y * Linesize + 2, (tail[i].x + 1) * Linesize - 1, (tail[i].y + 1) * Linesize - 1);
        }
        // 움직이는 꼬리 그리기
        for (int i = 0; i < mtailcount; i++)
        {
            SelectObject(mDC, MovingTail[i].color);
            Ellipse(mDC, MovingTail[i].x * Linesize, MovingTail[i].y * Linesize, (MovingTail[i].x + 1) * Linesize, (MovingTail[i].y + 1) * Linesize);
        }
        // 플레이어 꼬리 그리기
        for (int i = 0; i < ptailcount; i++)
        {
            SelectObject(mDC, Player_tail[i].color);
            Ellipse(mDC, Player_tail[i].x * Linesize + player.size, Player_tail[i].y * Linesize + player.size, (Player_tail[i].x + 1) * Linesize - player.size, (Player_tail[i].y + 1) * Linesize - player.size);
        }
        //장애물 그리기
        for (int i = 0; i < blockcount; i++)
        {
            SelectObject(mDC, blockcolor);
            Rectangle(mDC, Block[i].x * Linesize + 3, Block[i].y * Linesize + 3, (Block[i].x + 1) * Linesize - 3, (Block[i].y + 1) * Linesize - 3);
        }
        //a모드시 시작지점 그리기
        if (Block_OnOff == false)
        {
            SelectObject(mDC, StartingPoint.color);
            Rectangle(mDC, StartingPoint.x * Linesize, StartingPoint.y * Linesize, (StartingPoint.x + 1) * Linesize, (StartingPoint.y + 1) * Linesize);
        }
        //플레이어 그리기
        SelectObject(mDC, player.color);
        Ellipse(mDC, player.x * Linesize + player.size, player.y * Linesize + player.size, (player.x + 1) * Linesize - player.size, (player.y + 1) * Linesize - player.size);
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap);

        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            ///점프키 설정
            if (jump)
            {
                switch (jumptime)
                {
                case 0:
                    if (right)
                    {
                        player.y--;
                    }
                    else if (left) {
                        player.y--;
                    }
                    else if (up)
                    {
                        player.x++;
                    }
                    else if (down) {
                        player.x++;
                    }
                    jumptime++;
                    break;
                case 1:
                    if (right)
                    {
                        player.y--;
                    }
                    else if (left) {
                        player.y--;
                    }
                    else if (up)
                    {
                        player.x++;
                    }
                    else if (down) {
                        player.x++;
                    }
                    jumptime++;
                    break;
                case 2:
                    if (right)
                    {
                        player.y++;
                    }
                    else if (left) {
                        player.y++;
                    }
                    else if (up)
                    {
                        player.x--;
                    }
                    else if (down) {
                        player.x--;
                    }
                    jumptime++;
                    break;

                case 3:
                    if (right)
                    {
                        player.y++;
                    }
                    else if (left) {
                        player.y++;
                    }
                    else if (up)
                    {
                        player.x--;
                    }
                    else if (down) {
                        player.x--;
                    }
                    jump = false;
                    break;
                default:
                    break;
                }
            }
            // 플레이어 꼬리값 설정
            for (int i = ptailcount - 1; i > 0; i--)
            {
                Player_tail[i].x = Player_tail[i - 1].x;
                Player_tail[i].y = Player_tail[i - 1].y;
            }
            Player_tail[0].x = player.x;
            Player_tail[0].y = player.y;
            //상하좌우 이동설정
            if (right)
            {
                if (Block_OnOff)
                {
                    for (int i = 0; i < blockcount; i++)
                    {
                        if (player.x == Block[i].x - 1 && player.y == Block[i].y)
                        {
                            right = false;
                            left = true;
                            player.x--;
                        }
                    }
                }
                player.x++;
                if (player.x > Mapsize - 1)
                {
                    if (player.y == Mapsize - 1)
                    {
                        player.x--;
                        player.y--;
                        right = false;
                        up = true;
                    }
                    else {
                        player.x--;
                        player.y++;
                        left = true;
                        right = false;
                    }
                }
            }
            else if (left)
            {
                //장애물 충돌검사
                if (Block_OnOff)
                {
                    for (int i = 0; i < blockcount; i++)
                    {
                        if (player.x == Block[i].x + 1 && player.y == Block[i].y)
                        {

                            right = true;
                            left = false;
                            player.x++;
                        }
                    }
                }
                player.x--;

                if (player.x < 0)
                {
                    if (player.y == Mapsize - 1)
                    {
                        player.x++;
                        player.y--;
                        left = false;
                        up = true;
                    }
                    else
                    {
                        player.x++;
                        player.y++;
                        left = false;
                        right = true;
                    }
                }
            }
            else if (up)
            {
                if (Block_OnOff)
                {
                    for (int i = 0; i < blockcount; i++)
                    {
                        if (player.x == Block[i].x && player.y - 1 == Block[i].y)
                        {
                            up = false;
                            down = true;
                            player.y++;
                        }
                    }
                }
                player.y--;

                if (player.y < 0)
                {
                    if (player.x == Mapsize - 1)
                    {
                        player.x--;
                        player.y++;
                        up = false;
                        left = true;
                    }
                    else {
                        player.x++;
                        player.y++;
                        up = false;
                        down = true;
                    }
                }
            }
            else if (down)
            {
                if (Block_OnOff)
                {
                    for (int i = 0; i < blockcount; i++)
                    {
                        if (player.x == Block[i].x && player.y + 1 == Block[i].y)
                        {
                            up = true;
                            down = false;
                            player.y--;
                        }
                    }
                }
                player.y++;
                if (player.y > Mapsize - 1)
                {
                    if (player.x == Mapsize - 1)
                    {
                        player.x--;
                        player.y--;
                        left = true;
                        down = false;
                    }
                    else
                    {
                        player.x++;
                        player.y--;
                        up = true;
                        down = false;
                    }
                }
            }
            //움직이는 꼬리 구현
            if (TailMove)
            {
                for (int i = 0; i < mtailcount; i++)
                {
                    switch (MovingTail[i].size) {
                    case 0:
                        if (MovingTail[i].Right)
                        {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x + 1 == Block[i].x && MovingTail[i].y == Block[i].y)
                                {
                                    MovingTail[i].Right = false;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].x--;
                                }
                            }

                            MovingTail[i].x++;
                            if (MovingTail[i].x == Mapsize)
                            {
                                if (MovingTail[i].y == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y--;
                                    MovingTail[i].Right = false;
                                    MovingTail[i].Up = true;
                                }
                                else {
                                    MovingTail[i].x--;
                                    MovingTail[i].y++;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].Right = false;
                                }
                            }
                        }
                        else if (MovingTail[i].Left)
                        {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x + 1 && MovingTail[i].y == Block[i].y)
                                {

                                    MovingTail[i].Right = true;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].x++;
                                }
                            }
                            MovingTail[i].x--;

                            if (MovingTail[i].x < 0)
                            {
                                if (MovingTail[i].y == Mapsize - 1)
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y--;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].Up = true;
                                }
                                else
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y++;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].Right = true;
                                }
                            }
                        }
                        else if (MovingTail[i].Down) {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y + 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = true;
                                    MovingTail[i].Down = false;
                                    MovingTail[i].y--;
                                }
                            }

                            MovingTail[i].y++;
                            if (MovingTail[i].y == Mapsize)
                            {
                                if (MovingTail[i].x == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y--;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].Down = false;
                                }
                                else
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y--;
                                    MovingTail[i].Up = true;
                                    MovingTail[i].Down = false;
                                }
                            }
                        }
                        else if (MovingTail[i].Up)
                        {
                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y - 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Down = true;
                                    MovingTail[i].y++;
                                }
                            }
                            MovingTail[i].y--;
                            if (MovingTail[i].y < 0)
                            {
                                if (MovingTail[i].x == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y++;
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Left = true;
                                }
                                else {
                                    MovingTail[i].x++;
                                    MovingTail[i].y++;
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Down = true;
                                }
                            }
                        }
                        break;
                    case 1:
                        if (MovingTail[i].Right)
                        {
                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x - 1 && MovingTail[i].y == Block[i].y)
                                {
                                    MovingTail[i].Right = false;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].x--;
                                }
                            }
                            MovingTail[i].x++;
                            if (MovingTail[i].x == Mapsize)
                            {
                                if (MovingTail[i].y == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y--;
                                    MovingTail[i].Right = false;
                                    MovingTail[i].Up = true;
                                }
                                else {
                                    MovingTail[i].x--;
                                    MovingTail[i].y++;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].Right = false;
                                }
                            }
                        }
                        else if (MovingTail[i].Left)
                        {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x + 1 && MovingTail[i].y == Block[i].y)
                                {

                                    MovingTail[i].Right = true;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].x++;
                                }
                            }
                            MovingTail[i].x--;

                            if (MovingTail[i].x < 0)
                            {
                                if (MovingTail[i].y == Mapsize - 1)
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y--;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].Up = true;
                                }
                                else
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y++;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].Right = true;
                                }
                            }
                        }
                        else if (MovingTail[i].Down) {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y + 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = true;
                                    MovingTail[i].Down = false;
                                    MovingTail[i].y--;
                                }
                            }
                            MovingTail[i].y++;
                            if (MovingTail[i].y == Mapsize)
                            {
                                if (MovingTail[i].x == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y--;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].Down = false;
                                }
                                else
                                {
                                    MovingTail[i].x++;
                                    MovingTail[i].y--;
                                    MovingTail[i].Up = true;
                                    MovingTail[i].Down = false;
                                }
                            }
                        }
                        else if (MovingTail[i].Up)
                        {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y - 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Down = true;
                                    MovingTail[i].y++;
                                }
                            }
                            MovingTail[i].y--;
                            if (MovingTail[i].y < 0)
                            {
                                if (MovingTail[i].x == Mapsize - 1)
                                {
                                    MovingTail[i].x--;
                                    MovingTail[i].y++;
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Left = true;
                                }
                                else {
                                    MovingTail[i].x++;
                                    MovingTail[i].y++;
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Down = true;
                                }
                            }
                        }
                        break;
                    case 2:
                        if (MovingTail[i].Right)
                        {
                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x - 1 && MovingTail[i].y == Block[i].y)
                                {
                                    MovingTail[i].Right = false;
                                    MovingTail[i].Left = true;
                                    MovingTail[i].x--;
                                }
                            }
                            MovingTail[i].x++;
                            if (MovingTail[i].x == Mapsize - 2)
                            {

                                MovingTail[i].x--;
                                MovingTail[i].y++;
                                MovingTail[i].Right = false;
                                MovingTail[i].Down = true;
                            }
                        }
                        else if (MovingTail[i].Left)
                        {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x + 1 && MovingTail[i].y == Block[i].y)
                                {

                                    MovingTail[i].Right = true;
                                    MovingTail[i].Left = false;
                                    MovingTail[i].x++;
                                }
                            }
                            MovingTail[i].x--;
                            if (MovingTail[i].x < 2)
                            {
                                MovingTail[i].x++;
                                MovingTail[i].y--;
                                MovingTail[i].Left = false;
                                MovingTail[i].Up = true;
                            }

                        }
                        else if (MovingTail[i].Down) {

                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y + 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = true;
                                    MovingTail[i].Down = false;
                                    MovingTail[i].y--;
                                }
                            }
                            MovingTail[i].y++;
                            if (MovingTail[i].y == Mapsize - 2)
                            {

                                MovingTail[i].x--;
                                MovingTail[i].y--;
                                MovingTail[i].Left = true;
                                MovingTail[i].Down = false;
                            }
                        }
                        else if (MovingTail[i].Up)
                        {
                            for (int i = 0; i < blockcount; i++)
                            {
                                if (MovingTail[i].x == Block[i].x && MovingTail[i].y - 1 == Block[i].y)
                                {
                                    MovingTail[i].Up = false;
                                    MovingTail[i].Down = true;
                                    MovingTail[i].y++;
                                }
                            }
                            MovingTail[i].y--;
                            if (MovingTail[i].y < 2)
                            {
                                MovingTail[i].x++;
                                MovingTail[i].y++;
                                MovingTail[i].Up = false;
                                MovingTail[i].Right = true;
                            }
                        }
                    case 3:
                        break;
                    }
                }
            }

            //A 모드 시작 지점과 플레이어의 충돌검사
            if (Block_OnOff == false)
            {
                if (StartingPoint.x == player.x && StartingPoint.y == player.y) {
                    for (int i = 0; i < smcount; i++)
                    {
                        MovingTail[mtailcount].x = SaveMovingTail[i].x;
                        MovingTail[mtailcount].y = SaveMovingTail[i].y;
                        MovingTail[mtailcount].color = SaveMovingTail[i].color;
                        MovingTail[mtailcount].size = SaveMovingTail[i].size;
                        MovingTail[mtailcount].Up = SaveMovingTail[i].Up;
                        MovingTail[mtailcount].Down = SaveMovingTail[i].Down;
                        MovingTail[mtailcount].Left = SaveMovingTail[i].Left;
                        MovingTail[mtailcount].Right = SaveMovingTail[i].Right;
                        mtailcount++;
                    }
                    ptailcount = sptcount;
                    Block_OnOff = true;
                    MoveSpeed = SaveSpeed;
                    SetTimer(hWnd, 1, MoveSpeed, NULL);
                }
            }
            InvalidateRect(hWnd, NULL, false);
            break;
        case TIMER_RESTORE_SIZE:
            player.size = 0;
            KillTimer(hWnd, TIMER_RESTORE_SIZE);
            InvalidateRect(hWnd, NULL, false);
            break;
        }
        
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);

        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}