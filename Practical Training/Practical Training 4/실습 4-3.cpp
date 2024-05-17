#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <math.h >
#include "resource.h"
HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 4-3";
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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 850, 100, 377, 780, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);

    }
    return (int)Message.wParam;
}
bool IsRectOverlap(const RECT* rect1, const RECT* rect2) {
    // 첫 번째 사각형의 오른쪽이 두 번째 사각형의 왼쪽보다 왼쪽에 있거나,
    // 첫 번째 사각형의 왼쪽이 두 번째 사각형의 오른쪽보다 오른쪽에 있는 경우 겹치지 않음
    if (rect1->right <= rect2->left || rect1->left >= rect2->right) {
        return false;
    }

    // 첫 번째 사각형의 아래쪽이 두 번째 사각형의 위쪽보다 위에 있거나,
    // 첫 번째 사각형의 위쪽이 두 번째 사각형의 아래쪽보다 아래에 있는 경우 겹치지 않음
    if (rect1->bottom <= rect2->top || rect1->top >= rect2->bottom) {
        return false;
    }

    // 위 조건에 해당하지 않는 경우 두 사각형은 겹침
    return true;
}
BOOL InRectangle(RECT rect, int mx, int my) {
    if (mx <= rect.right && mx >= rect.left && my <= rect.bottom && my >= rect.top)
    {
        return true;
    }
    return false;
}
float LengthPts(int x1, int y1, int x2, int y2)
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

struct Box {
    RECT rect;
    int direct;
    int status;
    int x;
    int y;
    double theta;
    int shape;
    HBRUSH color;
    int size;
};
#define MakeBlock 10001
#define MoveBlock 10002
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hDC, mDC;
    static HBITMAP hBitmap;
    static RECT rt;
    static Box Map[9][19],Curfig[3][3],temp[3][3];
    static HBRUSH blockcolor[8];
    int mx, my;
    TCHAR Clear1[300];
    static bool Getback = false, setblock = false,RemoveLinebyColor= false;
    static  int RemoveLine{ 0 }, blocksize{ 40 }, maxcolor{ 8 }, maxshape{ 6 }, Decideshape, makecount{ 0 }, count{0};
    switch (uMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rt); //--- 윈도우 크기 얻어오기
        static HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
        blockcolor[0] = CreateSolidBrush(RGB(255, 0, 0));
        blockcolor[1] = CreateSolidBrush(RGB(255, 127, 0));
        blockcolor[2] = CreateSolidBrush(RGB(255, 255, 0));
        blockcolor[3] = CreateSolidBrush(RGB(0, 255, 0));
        blockcolor[4] = CreateSolidBrush(RGB(120, 120, 255));
        blockcolor[5] = CreateSolidBrush(RGB(0, 0, 255));
        blockcolor[6] = CreateSolidBrush(RGB(180, 40, 255));
        blockcolor[7] = CreateSolidBrush(RGB(0, 0, 0));
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 19; j++)
            {

                if (j != 18)
                {
                    Map[i][j].rect.left = i * blocksize;
                    Map[i][j].rect.right = (i + 1) * blocksize;
                    Map[i][j].rect.top = j * blocksize;
                    Map[i][j].rect.bottom = (j + 1) * blocksize;
                    Map[i][j].color = white;
                    Map[i][j].status = 0;
                }
                else {
                    Map[i][j].rect.left = i * blocksize;
                    Map[i][j].rect.right = (i + 1) * blocksize;
                    Map[i][j].rect.top = j * blocksize;
                    Map[i][j].rect.bottom = (j + 1) * blocksize;
                    Map[i][j].color = white;
                    Map[i][j].status = 1;
                }
            }
            
        }
        break;
    case WM_LBUTTONDOWN:
        hDC = GetDC(hWnd);
        InvalidateRect(hWnd, NULL, false);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        if (wParam == VK_SPACE)
        {
            if (Decideshape != 5)
            {
                // 3 x 3 도형 돌리기
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        temp[i][j].status = Curfig[i][j].status;
                        temp[i][j].color = Curfig[i][j].color;
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[j][2 - i].status = temp[i][j].status;
                        Curfig[j][2 - i].color = temp[i][j].color;
                    }
                }
                // 변경후 블록의 맵 이탈 검사
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (Curfig[i][j].status == 1)
                        {
                            RECT CheckInmap2 = { 0,0,360,720 };
                           
                            if (!IsRectOverlap(&Curfig[i][j].rect, &CheckInmap2))
                            {
                                Getback = true;
                            }
                        }
                    }
                }
                //움직인 이후 Map과 검사
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (Curfig[i][j].status == 1)
                        {
                            for (int a = 0; a < 9; a++)
                            {
                                for (int b = 0; b < 19; b++)
                                {
                                    if (Map[a][b].status == 1 && IsRectOverlap(&Map[a][b].rect, &Curfig[i][j].rect))
                                    {
                                        Getback = true;
                                    }
                                }
                            }
                        }
                    }
                }
                //넘어가면 원래대로 돌려둠
                if (Getback)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            Curfig[i][j].status = temp[i][j].status;
                            Curfig[i][j].color = temp[i][j].color;
                        }
                    }
                    Getback = false;
                }
            }
            // 2 x 2 도형 돌리기
            else
            {
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        temp[i][j].status = Curfig[i][j].status;
                        temp[i][j].color = Curfig[i][j].color;
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        Curfig[j][1 - i].status = temp[i][j].status;
                        Curfig[j][1 - i].color = temp[i][j].color;
                    }
                }
            }
        }
        else if (wParam == VK_DOWN)
        {
            //이동
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.top += blocksize;
                    Curfig[i][j].rect.bottom += blocksize;
                }
            }
            //움직인 이후 Map과 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        for (int a = 0; a < 9; a++)
                        {
                            for (int b = 0; b < 19; b++)
                            {
                                if (Map[a][b].status == 1 && IsRectOverlap(&Map[a][b].rect, &Curfig[i][j].rect))
                                {
                                    setblock = true;
                                }
                            }
                        }
                    }
                }
            }
            if (setblock)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].rect.top -= blocksize;
                        Curfig[i][j].rect.bottom -= blocksize;
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (Curfig[i][j].status == 1)
                        {
                            for (int a = 0; a < 9; a++)
                            {
                                for (int b = 0; b < 18; b++)
                                {
                                    if (Map[a][b].rect.left == Curfig[i][j].rect.left && Map[a][b].rect.right == Curfig[i][j].rect.right && Map[a][b].rect.top == Curfig[i][j].rect.top && Map[a][b].rect.bottom == Curfig[i][j].rect.bottom)
                                    {
                                        Map[a][b].status = 1;
                                        Map[a][b].color = Curfig[i][j].color;
                                    }
                                }
                            }
                        }
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].status = 0;
                    }
                }
                setblock = false;
            }
            //블럭을 움직이지 않는 상태 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 0)
                    {
                        makecount++;
                        if (makecount == 9)
                        {
                            KillTimer(hWnd, MakeBlock);
                            SetTimer(hWnd, MakeBlock, 500, NULL);
                        }
                    }
                }
            }
            makecount = 0;
        }
        else if (wParam == VK_LEFT)
        {
            //이동
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.left -= blocksize;
                    Curfig[i][j].rect.right -= blocksize;
                }
            }
            // 변경후 블록의 맵 이탈 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        RECT CheckInmap2 = { 0,0,360,720 };

                        if (!IsRectOverlap(&Curfig[i][j].rect, &CheckInmap2))
                        {
                            Getback = true;
                        }
                    }
                }
            }
            //움직인 이후 Map과 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        for (int a = 0; a < 9; a++)
                        {
                            for (int b = 0; b < 19; b++)
                            {
                                if (Map[a][b].status == 1 && IsRectOverlap(&Map[a][b].rect, &Curfig[i][j].rect))
                                {
                                    Getback = true;
                                }
                            }
                        }
                    }
                }
            }
            //넘어가면 원래대로 돌려둠
            if (Getback)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].rect.left += blocksize;
                        Curfig[i][j].rect.right += blocksize;
                    }
                }
                Getback = false;
            }

        }
        else if (wParam == VK_RIGHT)
        {
            //이동
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.left += blocksize;
                    Curfig[i][j].rect.right += blocksize;
                }
            }
            // 변경후 블록의 맵 이탈 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        RECT CheckInmap2 = { 0,0,360,720 };

                        if (!IsRectOverlap(&Curfig[i][j].rect, &CheckInmap2))
                        {
                            Getback = true;
                        }
                    }
                }
            }
            //움직인 이후 Map과 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        for (int a = 0; a < 9; a++)
                        {
                            for (int b = 0; b < 19; b++)
                            {
                                if (Map[a][b].status == 1 && IsRectOverlap(&Map[a][b].rect, &Curfig[i][j].rect))
                                {
                                    Getback = true;
                                }
                            }
                        }
                    }
                }
            }
            //넘어가면 원래대로 돌려둠
            if (Getback)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].rect.left -= blocksize;
                        Curfig[i][j].rect.right -= blocksize;
                    }
                }
                Getback = false;
            }
        }
        else if (wParam == 'h'|| wParam == 'H')
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        Curfig[i][j].color = blockcolor[6];
                    }
                }
            }
        }
        InvalidateRect(hWnd, NULL, false);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC); //--- 메모리 DC 만들기
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기

        SelectObject(mDC, (HBITMAP)hBitmap); //--- 메모리 DC와 비트맵 연결하기
        Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- 화면에 비어있기 때문에 화면 가득히 사각형을 그려 배경색으로 설정하기
        //--- 모든 그리기를 메모리 DC에 한다.
        
        //맵 그리기
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                SelectObject(mDC,Map[i][j].color);
                Rectangle(mDC, Map[i][j].rect.left, Map[i][j].rect.top, Map[i][j].rect.right, Map[i][j].rect.bottom);
            }
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (Curfig[i][j].status == 1)
                {
                    SelectObject(mDC, Curfig[i][j].color);
                    Rectangle(mDC, Curfig[i][j].rect.left, Curfig[i][j].rect.top, Curfig[i][j].rect.right, Curfig[i][j].rect.bottom);
                }
            }
        }
        //--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- 생성한 메모리 DC 삭제
        DeleteObject(hBitmap); //--- 생성한 비트맵 삭제
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case MoveBlock:
           
            // 아래로 계속 내리기
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.top += blocksize;
                    Curfig[i][j].rect.bottom += blocksize;
                }
            }
            //움직인 이후 Map과 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 1)
                    {
                        for (int a = 0; a < 9; a++)
                        {
                            for (int b = 0; b < 19; b++)
                            {
                                if (Map[a][b].status == 1 && IsRectOverlap(&Map[a][b].rect, &Curfig[i][j].rect))
                                {
                                    setblock = true;
                                }
                            }
                        }
                    }
                }
            }
            if (setblock)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].rect.top -= blocksize;
                        Curfig[i][j].rect.bottom -= blocksize;
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (Curfig[i][j].status == 1)
                        {
                            for (int a = 0; a < 9; a++)
                            {
                                for (int b = 0; b < 18; b++)
                                {
                                    if (Map[a][b].rect.left == Curfig[i][j].rect.left&& Map[a][b].rect.right == Curfig[i][j].rect.right && Map[a][b].rect.top == Curfig[i][j].rect.top && Map[a][b].rect.bottom == Curfig[i][j].rect.bottom)
                                    {
                                        Map[a][b].status = 1;
                                        Map[a][b].color = Curfig[i][j].color;
                                    }
                                }
                            }
                        }
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        Curfig[i][j].status = 0;
                    }
                }
                setblock = false;
            }

            //블럭을 움직이지 않는 상태 검사
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (Curfig[i][j].status == 0)
                    {
                        makecount++;
                        if (makecount == 9)
                        {
                            SetTimer(hWnd, MakeBlock, 500, NULL);
                        }
                    }
                }
            }
            makecount = 0;
            break;
            //블럭 리젠
        case MakeBlock:
            // 한줄이 다 차면 없애기
            for (int i = 0; i < 18; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (Map[j][i].status == 1)
                    {
                        RemoveLine++;
                    }
                }
                if (RemoveLine == 9)
                {
                    for (int a = 0; a < 9; a++)
                    {
                        for (int b = i; b > 0; b--)
                        {
                            Map[a][b].status = Map[a][b - 1].status;
                            Map[a][b].color = Map[a][b - 1].color;
                        }
                    }
                    for (int a = 0; a < 9; a++)
                    {
                        Map[a][0].status = 0;
                        Map[a][0].color = white;
                    }
                }
                RemoveLine = 0;
            }
            //상하로 3개의 색이 같으면 사라지기
            for (int i = 0; i < 9; i++)
            {
                for (int j = 17; j > 0; j--)
                {
                    if (Map[i][j].color == Map[i][j - 1].color && Map[i][j].color == Map[i][j - 2].color && Map[i][j].status == 1 && Map[i][j - 1].status == 1 && Map[i][j - 2].status == 1)
                    {
                        RemoveLinebyColor = true;
                    }
                    if (RemoveLinebyColor)
                    {
                        for (int b = j; b > 0; b--)
                        {
                            Map[i][b].status = Map[i][b - 3].status;
                            Map[i][b].color = Map[i][b - 3].color;
                        }
                        for (int b = 2; b >= 0; b--)
                        {
                            Map[i][b].status = 0;
                            Map[i][b].color = white;
                        }
                        RemoveLinebyColor = false;
                    }
                }
            }
            //좌로 3개의 색이 같으면 사라지기
            for (int i = 17; i >= 0; i--)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (Map[j][i].color == Map[j + 1][i].color && Map[j][i].color == Map[j + 2][i].color && Map[j][i].status == 1&& Map[j+1][i].status == 1&& Map[j+2][i].status == 1)
                    {
                        RemoveLinebyColor = true;
                    }
                    if (RemoveLinebyColor)
                    {
                        for (int a = j; a <= j+2; a++)
                        {
                            for (int b = i; b > 0; b--)
                            {
                                Map[a][b].status = Map[a][b - 1].status;
                                Map[a][b].color = Map[a][b - 1].color;
                            }
                        }
                        RemoveLinebyColor = false;
                    }
                }
            }

            Decideshape = rand() % maxshape;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.left = 160 + i * blocksize;
                    Curfig[i][j].rect.right = 160 + (i + 1) * blocksize;
                    Curfig[i][j].rect.top = j * blocksize;
                    Curfig[i][j].rect.bottom = (j + 1) * blocksize;
                    Curfig[i][j].color = white;
                    Curfig[i][j].status = 0;
                }
            }
            switch (Decideshape)
            {
                
            case 0: // 3개 일자
                for (int i = 0; i < 3; i++)
                {
                    Curfig[1][i].status = 1;
                    Curfig[1][i].color = blockcolor[rand()% maxcolor];
                }
                break;
            case 1:
                Curfig[1][0].status = 1;
                Curfig[1][1].status = 1;
                Curfig[2][1].status = 1;
                Curfig[2][2].status = 1;
                Curfig[1][0].color = blockcolor[rand() % maxcolor];
                Curfig[1][1].color = blockcolor[rand() % maxcolor];
                Curfig[2][1].color = blockcolor[rand() % maxcolor];
                Curfig[2][2].color = blockcolor[rand() % maxcolor];
                break;
            case 2:
                Curfig[1][0].status = 1;
                Curfig[1][1].status = 1;
                Curfig[1][2].status = 1;
                Curfig[2][0].status = 1;
                Curfig[1][0].color = blockcolor[rand() % maxcolor];
                Curfig[1][1].color = blockcolor[rand() % maxcolor];
                Curfig[1][2].color = blockcolor[rand() % maxcolor];
                Curfig[2][0].color = blockcolor[rand() % maxcolor];
                break;
            case 3:
                Curfig[1][0].status = 1;
                Curfig[1][1].status = 1;
                Curfig[1][2].status = 1;
                Curfig[2][0].status = 1;
                Curfig[2][2].status = 1;
                Curfig[1][0].color = blockcolor[rand() % maxcolor];
                Curfig[1][1].color = blockcolor[rand() % maxcolor];
                Curfig[1][2].color = blockcolor[rand() % maxcolor];
                Curfig[2][0].color = blockcolor[rand() % maxcolor];
                Curfig[2][2].color = blockcolor[rand() % maxcolor];
                break;
            case 4:
                Curfig[1][0].status = 1;
                Curfig[1][1].status = 1;
                Curfig[1][2].status = 1;
                Curfig[2][1].status = 1;
                Curfig[1][0].color = blockcolor[rand() % maxcolor];
                Curfig[1][1].color = blockcolor[rand() % maxcolor];
                Curfig[1][2].color = blockcolor[rand() % maxcolor];
                Curfig[2][1].color = blockcolor[rand() % maxcolor];
                break;
            case 5:
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        Curfig[i][j].status = 1;
                        Curfig[i][j].color = blockcolor[rand() % maxcolor];
                    }
                }
                break;
            default:
                break;
            }
            KillTimer(hWnd, MakeBlock);
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false); //--- 화면에 다시그리기를 할 때 기존의 내용을 삭제하지 않는다.
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_GAME_START:
            SetTimer(hWnd, MakeBlock, 500, NULL);
            SetTimer(hWnd, MoveBlock, 1000, NULL);
            break;
        case ID_GAME_END:
            SendMessage(hWnd, WM_DESTROY, NULL, NULL);
            break;
        case ID_BLOCK_4:
            maxshape = 4;
            break;
        case ID_BLOCK_5:
            maxshape = 5;
            break;
        case ID_BLOCK_6:
            maxshape = 6;
            break;
        case ID_COLOR_6:
            maxcolor = 6;
            break;
        case ID_COLOR_7:
            maxcolor = 7;
            break;
        case ID_COLOR_8:
            maxcolor = 8;
            break;
        case ID_SPEED_FAST:
            KillTimer(hWnd, MoveBlock);
            SetTimer(hWnd, MoveBlock, 200, NULL);
            break;
        case ID_SPEED_MEDIUM:
            KillTimer(hWnd, MoveBlock);
            SetTimer(hWnd, MoveBlock, 500, NULL);
            break;
        case ID_SPEED_SLOW:
            KillTimer(hWnd, MoveBlock);
            SetTimer(hWnd, MoveBlock, 800, NULL);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        KillTimer(hWnd, MakeBlock);
        KillTimer(hWnd, MoveBlock);
        DeleteObject(white);
        for (int i = 0; i < 8; i++)
        {
            DeleteObject(blockcolor[i]);
        }
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
