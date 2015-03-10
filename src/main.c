/*
*	main.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	10.3.2015
*	redistributable under the terms of the GNU/GPLv3
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "tetris.h"
#include "graph.h"
#include "resource.h"

 

#define _MainClassName TEXT("WinAPIMainClass")
#define _AppName TEXT("Tetris")
#define _TimerClock 1

HINSTANCE g_hInstance;
HWND g_hwndMain;
MSG msg;
TETRIS_T g_tetris;

static BOOL InitApp();
static LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void onPaint();
static void paintGrid(HDC hdc);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow) {
        g_hInstance = hInstance;
        
        if (!InitApp())
                return FALSE;
                
        while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
        
        return msg.wParam;
}


BOOL InitApp() {
        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDR_MAIN));
        wc.hIconSm = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDR_MAIN));
        wc.hInstance = g_hInstance;
        wc.lpfnWndProc = WindowProcMain;
        wc.lpszClassName = _MainClassName;
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        
        if (!RegisterClassEx(&wc))
                return FALSE;
        
        g_hwndMain = CreateWindowEx(0, // rozšíøený styl okna    
                _MainClassName, // jméno tøídy
                _AppName, // text okna
                WS_OVERLAPPEDWINDOW | WS_VISIBLE, // styl okna
                100, 100, // souøadnice na obraziovce
                450, 350, // rozmìry - šíøka a výška
                (HWND)NULL, // okna pøedka
                NULL, // handle hlavní nabídky
                g_hInstance, // handle instance
                NULL); // další "uživatelská" data
                
        if ( g_hwndMain == NULL )
                return FALSE;
        
        t_create_game(&g_tetris, 10, 20);
                
        ShowWindow(g_hwndMain, SW_SHOWNORMAL);
        UpdateWindow(g_hwndMain);

        SetTimer(g_hwndMain, _TimerClock, 600, NULL);
                                  
        return TRUE;
}


LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        HDC hdc;
        TCHAR chText[100];
        
        switch (uMsg) {
                case WM_KEYDOWN:
                        hdc = GetDC(g_hwndMain);
                        switch (wParam) {        
                                case VK_UP:
                                        t_rotate(hdc, &g_tetris);
                                        break;
                                case VK_DOWN:
                                        while(t_move_down(hdc, &g_tetris) != -1)
                                                ;
                                        break;
                                case VK_LEFT:
                                        t_move_left(hdc, &g_tetris);
                                        break;
                                case VK_RIGHT:
                                        t_move_right(hdc, &g_tetris);
                                        break;                        
                        }
                        ReleaseDC(g_hwndMain, hdc);
                        break;
                case WM_TIMER:
                        hdc = GetDC(g_hwndMain);
                        t_go(hdc, &g_tetris);
                        ReleaseDC(g_hwndMain, hdc);
                        _stprintf(chText, "Tetris - score: %d", g_tetris.score);
                        SetWindowText(g_hwndMain, chText);
                        break;
                case WM_PAINT:
                        onPaint();
                        break;
                case WM_DESTROY:
                        PostQuitMessage(0);
                        break;
                case WM_CLOSE:
                        if ( MessageBox(hwnd, "Opravdu konèit?", _AppName,
                                MB_YESNO | MB_ICONQUESTION) != IDYES )
                                        return 0;
                        t_delete_game(&g_tetris);
                        break;
        }
        
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void onPaint() {
        PAINTSTRUCT ps;
        HDC hdc;
        
        hdc = BeginPaint(g_hwndMain, &ps);
        paintGrid(hdc); 
        EndPaint(g_hwndMain, &ps);
}


void paintGrid(HDC hdc) {
        RECT rect;
        
        GetClientRect(g_hwndMain, &rect);
        g_draw_mesh(hdc, &g_tetris, rect);
}

