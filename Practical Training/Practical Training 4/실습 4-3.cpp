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
    // ù ��° �簢���� �������� �� ��° �簢���� ���ʺ��� ���ʿ� �ְų�,
    // ù ��° �簢���� ������ �� ��° �簢���� �����ʺ��� �����ʿ� �ִ� ��� ��ġ�� ����
    if (rect1->right <= rect2->left || rect1->left >= rect2->right) {
        return false;
    }

    // ù ��° �簢���� �Ʒ����� �� ��° �簢���� ���ʺ��� ���� �ְų�,
    // ù ��° �簢���� ������ �� ��° �簢���� �Ʒ��ʺ��� �Ʒ��� �ִ� ��� ��ġ�� ����
    if (rect1->bottom <= rect2->top || rect1->top >= rect2->bottom) {
        return false;
    }

    // �� ���ǿ� �ش����� �ʴ� ��� �� �簢���� ��ħ
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
        GetClientRect(hWnd, &rt); //--- ������ ũ�� ������
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
                // 3 x 3 ���� ������
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
                // ������ ����� �� ��Ż �˻�
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
                //������ ���� Map�� �˻�
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
                //�Ѿ�� ������� ������
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
            // 2 x 2 ���� ������
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
            //�̵�
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.top += blocksize;
                    Curfig[i][j].rect.bottom += blocksize;
                }
            }
            //������ ���� Map�� �˻�
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
            //���� �������� �ʴ� ���� �˻�
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
            //�̵�
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.left -= blocksize;
                    Curfig[i][j].rect.right -= blocksize;
                }
            }
            // ������ ����� �� ��Ż �˻�
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
            //������ ���� Map�� �˻�
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
            //�Ѿ�� ������� ������
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
            //�̵�
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.left += blocksize;
                    Curfig[i][j].rect.right += blocksize;
                }
            }
            // ������ ����� �� ��Ż �˻�
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
            //������ ���� Map�� �˻�
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
            //�Ѿ�� ������� ������
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
        mDC = CreateCompatibleDC(hDC); //--- �޸� DC �����
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom); //--- �޸� DC�� ������ ��Ʈ�� �����

        SelectObject(mDC, (HBITMAP)hBitmap); //--- �޸� DC�� ��Ʈ�� �����ϱ�
        Rectangle(mDC, 0, 0, rt.right, rt.bottom); //--- ȭ�鿡 ����ֱ� ������ ȭ�� ������ �簢���� �׷� �������� �����ϱ�
        //--- ��� �׸��⸦ �޸� DC�� �Ѵ�.
        
        //�� �׸���
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
        //--- �������� �޸� DC�� ������ ȭ�� DC�� �����Ѵ�.
        BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC); //--- ������ �޸� DC ����
        DeleteObject(hBitmap); //--- ������ ��Ʈ�� ����
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case MoveBlock:
           
            // �Ʒ��� ��� ������
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Curfig[i][j].rect.top += blocksize;
                    Curfig[i][j].rect.bottom += blocksize;
                }
            }
            //������ ���� Map�� �˻�
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

            //���� �������� �ʴ� ���� �˻�
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
            //�� ����
        case MakeBlock:
            // ������ �� ���� ���ֱ�
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
            //���Ϸ� 3���� ���� ������ �������
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
            //�·� 3���� ���� ������ �������
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
                
            case 0: // 3�� ����
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
        InvalidateRect(hWnd, NULL, false); //--- ȭ�鿡 �ٽñ׸��⸦ �� �� ������ ������ �������� �ʴ´�.
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
