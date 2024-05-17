#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Window Class Name";
LPCWSTR lpszWindowName = L"Window programming 2-9";
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
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 400, 100, 900, 900, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}
struct figure
{
    int x;
    int y;
    int size;
};
figure xyPos[4] = { {100,400,50 } ,{ 400,700,50 },{ 400,100,50 },{ 700,400,50 } };
bool fistposition = true;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    HDC hDC;
    PAINTSTRUCT ps;
    static SIZE size;
    static bool firstposition = true;
    RECT rect;
    RECT center = { 300,300,600,600 }; //  중앙  rect값 받아두기
    // 시작 x,y,size좌표
    figure temp[4];

    figure triangle = { xyPos[0].x,xyPos[0].y,xyPos[0].size };
    figure circle = { xyPos[1].x,xyPos[1].y,xyPos[1].size };
    figure sandclock = { xyPos[2].x,xyPos[2].y,xyPos[2].size };
    figure pentagon = { xyPos[3].x,xyPos[3].y,xyPos[3].size };

    POINT point[3] = { {triangle.x + triangle.size,triangle.y},
                             {triangle.x,triangle.y + 2 * triangle.size},
                              {triangle.x + 2 * triangle.size,triangle.y + 2 * triangle.size} };
    POINT sand[5] = { {sandclock.x,sandclock.y},
    {sandclock.x + 2 * sandclock.size,sandclock.y},
    {sandclock.x,sandclock.y + 2 * sandclock.size},
    {sandclock.x + 2 * sandclock.size,sandclock.y + 2 * sandclock.size},
    {sandclock.x,sandclock.y} };
    POINT ogag[5] = { {pentagon.x + pentagon.size,pentagon.y},
    {pentagon.x + 2 * pentagon.size,pentagon.y + 2 * pentagon.size / 3 },
    {pentagon.x + 3 * pentagon.size / 2,pentagon.y + 2 * pentagon.size},
    {pentagon.x + pentagon.size / 2,pentagon.y + 2 * pentagon.size},
    {pentagon.x,pentagon.y + 2 * pentagon.size / 3} };
    COLORREF rndbkclr = RGB(rand() % 256, rand() % 256, rand() % 256);
    switch (iMsg)
    {
    case WM_CREATE:
        srand(time(NULL));
        static HBRUSH common = CreateSolidBrush(RGB(100, 100, 100));
        static HBRUSH penta = CreateSolidBrush(RGB(255, 0, 0));
        static HBRUSH tri = CreateSolidBrush(RGB(0, 0, 255));
        static HBRUSH more = CreateSolidBrush(RGB(0, 255, 0));
        static HBRUSH cir = CreateSolidBrush(RGB(255, 200, 200));
        for (int j = 0; j < 30; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                int rnd = rand() % 4;
                temp[i] = xyPos[i];
                xyPos[i] = xyPos[rnd];
                xyPos[rnd] = temp[i];
            }
        }

        static int centershape = rand() % 4 + 1;
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);
        if (wParam == 't' || wParam == 'T') {

        }        if (wParam == 'q' || wParam == 'Q') {
            PostQuitMessage(0);
        }
        else if (wParam == 's' || wParam == 'S') {

            centershape = 4;
            more = CreateSolidBrush(rndbkclr);
        }
        else if (wParam == 'p' || wParam == 'P') {

            centershape = 3;
            penta = CreateSolidBrush(rndbkclr);
        }
        else if (wParam == 't' || wParam == 'T') {
            centershape = 2;
            tri = CreateSolidBrush(rndbkclr);
        }
        else if (wParam == 'e' || wParam == 'E') {
            centershape = 1;
            cir = CreateSolidBrush(rndbkclr);
            
        }


        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        hDC = GetDC(hWnd);
        if (wParam == VK_RIGHT) {
            figure last = xyPos[3];
            for (int i = 3; i > 0; --i) {
                xyPos[i] = xyPos[i - 1];
            }
            xyPos[0] = last;
        }
        else if (wParam == VK_LEFT) {
            figure first = xyPos[0];
            for (int i = 0; i < 3; ++i) {
                xyPos[i] = xyPos[i + 1];
            }
            xyPos[3] = first;
        
        }
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_KEYUP:
        hDC = GetDC(hWnd);
        if (wParam == 's' || wParam == 'S') {
            more = CreateSolidBrush(RGB(0, 255, 0));
            if ((sandclock.x == 400) && (sandclock.y == 100))
            {
                centershape = 4;
            }else if((pentagon.x == 400) && (pentagon.y == 100))
            {
                centershape = 3;
            }else if (triangle.x == 400 && triangle.y == 100)
            {
                centershape = 2;
            }
            else if ((circle.x == 400) && (circle.y == 100))
            {
                centershape = 1;
            }
        }
        else if (wParam == 'p' || wParam == 'P') {
            penta = CreateSolidBrush(RGB(255, 0, 0));
            if ((sandclock.x == 400) && (sandclock.y == 100))
            {
                centershape = 4;
            }
            else if ((pentagon.x == 400) && (pentagon.y == 100))
            {
                centershape = 3;
            }
            else if (triangle.x == 400 && triangle.y == 100)
            {
                centershape = 2;
            }
            else if ((circle.x == 400) && (circle.y == 100))
            {
                centershape = 1;
            }
            
        }
        else if (wParam == 't' || wParam == 'T') {
            tri = CreateSolidBrush(RGB(0, 0, 255));
            if ((sandclock.x == 400) && (sandclock.y == 100))
            {
                centershape = 4;
            }
            else if ((pentagon.x == 400) && (pentagon.y == 100))
            {
                centershape = 3;
            }
            else if (triangle.x == 400 && triangle.y == 100)
            {
                centershape = 2;
            }
            else if ((circle.x == 400) && (circle.y == 100))
            {
                centershape = 1;
            }
        }
        else if (wParam == 'e' || wParam == 'E') {
            cir = CreateSolidBrush(RGB(255, 200, 200));
            if ((sandclock.x == 400) && (sandclock.y == 100))
            {
                centershape = 4;
            }
            else if ((pentagon.x == 400) && (pentagon.y == 100))
            {
                centershape = 3;
            }
            else if (triangle.x == 400 && triangle.y == 100)
            {
                centershape = 2;
            }
            else if ((circle.x == 400) && (circle.y == 100))
            {
                centershape = 1;
            }
        }
        
        ReleaseDC(hWnd, hDC);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        rect = { 300, 300, 600, 600 };
     
        FrameRect(hDC, &rect, common); // 중앙 사각형 프레임

        SelectObject(hDC, more);// 모래시계 
        Polygon(hDC, sand, 5);

        SelectObject(hDC, penta);
        Polygon(hDC, ogag, 5); // 오각형

        SelectObject(hDC, cir);// 먹힌 원
        Pie(hDC, circle.x, circle.y, circle.x + 2 * circle.size, circle.y + 2 * circle.size, circle.x + circle.size, circle.y, circle.x + 2 * circle.size, circle.y + circle.size); // 먹힌 원

        SelectObject(hDC, tri);// 삼각형
        Polygon(hDC, point, 3);
        switch (centershape) // 중앙 공간 도형그리기
        {
        case 1: //파이 그리기
            SelectObject(hDC, cir);
            circle = { 300,300,150 };
            Pie(hDC, circle.x, circle.y, circle.x + 2 * circle.size, circle.y + 2 * circle.size, circle.x + circle.size, circle.y, circle.x + 2 * circle.size, circle.y + circle.size);
            break;
        case 2: // 삼각형 그리기
            triangle = { 300, 300, 150};
            point[0] = { triangle.x + triangle.size, triangle.y };
            point[1] = { triangle.x, triangle.y + 2 * triangle.size };
            point[2] = { triangle.x + 2 * triangle.size, triangle.y + 2 * triangle.size };
            SelectObject(hDC, tri);
            Polygon(hDC, point, 3);
            break;
        case 3:// 오각형 그리기
            pentagon = { 300,300,150 };
            ogag[0] = { pentagon.x + pentagon.size,pentagon.y };
            ogag[1] = { pentagon.x + 2 * pentagon.size,pentagon.y + 2 * pentagon.size / 3 };
            ogag[2] = { pentagon.x + 3 * pentagon.size / 2,pentagon.y + 2 * pentagon.size };
            ogag[3] = { pentagon.x + pentagon.size / 2,pentagon.y + 2 * pentagon.size };
            ogag[4] = { pentagon.x,pentagon.y + 2 * pentagon.size / 3 };
            SelectObject(hDC, penta);
            Polygon(hDC, ogag, 5);
            break;
        case 4: // 모래시계 그리기
            sandclock = { 300,300,150 };
            sand[0] = { sandclock.x,sandclock.y };
            sand[1] = { sandclock.x + 2 * sandclock.size,sandclock.y };
            sand[2] = { sandclock.x,sandclock.y + 2 * sandclock.size };
            sand[3] = { sandclock.x + 2 * sandclock.size,sandclock.y + 2 * sandclock.size };
            sand[4] = { sandclock.x,sandclock.y };
            SelectObject(hDC, more);// 모래시계 
            Polygon(hDC, sand, 5);
            break;
        default:
            break;
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        DeleteObject(penta);
        DeleteObject(tri);
        DeleteObject(common);
        DeleteObject(more);
        DeleteObject(cir);
        PostQuitMessage(0);
        break;

    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
