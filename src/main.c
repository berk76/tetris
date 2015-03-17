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
HWND g_hwndStatusBar;
MSG msg;
TETRIS_T g_tetris;

static BOOL InitApp();
static LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void startGame(TETRIS_T *tetris, int x_size, int y_size, int brick_size);
static void pauseGame(BOOL b);
static void onPaint();


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
        
        if (!RegisterClassEx(&wc)) {
                MessageBox(NULL, TEXT("This program requires Windows NT."), _AppName, MB_ICONERROR);
                return FALSE;
        }
        
        g_hwndMain = CreateWindowEx(0, // rozšíøený styl okna    
                _MainClassName, // jméno tøídy
                _AppName, // text okna
                WS_OVERLAPPEDWINDOW | WS_VISIBLE, // styl okna
                100, 100, // souøadnice na obraziovce
                450, 350, // rozmìry - šíøka a výška
                (HWND)NULL, // okna pøedka
                LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MAINMENU)), // handle hlavní nabídky
                g_hInstance, // handle instance
                NULL); // další "uživatelská" data
                
        if (g_hwndMain == NULL)
                return FALSE;
                
                
        g_hwndStatusBar = CreateWindowEx(0,
                TEXT("msctls_statusbar32"),
                TEXT("Stavový øádek"),
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0,
                g_hwndMain,
                (HMENU)NULL,
                g_hInstance,
                NULL);
        if ( g_hwndStatusBar == NULL )
                return FALSE;

        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(g_hwndStatusBar, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
        
        t_create_game(&g_tetris, 10, 20, 4);                
        ShowWindow(g_hwndMain, SW_SHOWNORMAL);
        UpdateWindow(g_hwndMain);
                                  
        return TRUE;
}


LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        HDC hdc;
        TCHAR chText[100];
        int ret;
        
        switch (uMsg) {
                case WM_KEYDOWN:
                        hdc = GetDC(g_hwndMain);
                        switch (wParam) {        
                                case VK_UP:
                                        pauseGame(FALSE);
                                        t_rotate(hdc, &g_tetris);
                                        break;
                                case VK_DOWN:
                                        pauseGame(FALSE);
                                        while(t_move_down(hdc, &g_tetris) != -1)
                                                ;
                                        break;
                                case VK_LEFT:
                                        pauseGame(FALSE);
                                        t_move_left(hdc, &g_tetris);
                                        break;
                                case VK_RIGHT:
                                        pauseGame(FALSE);
                                        t_move_right(hdc, &g_tetris);
                                        break;                        
                        }
                        ReleaseDC(g_hwndMain, hdc);
                        break;
                case WM_COMMAND:
                        switch (LOWORD(wParam)) {
                                case ID_G_TETRIS:
                                        startGame(&g_tetris, 10, 20, 4);
                                        break;
                                case ID_G_PENTIS:
                                        startGame(&g_tetris, 20, 20, 5);
                                        break;
                                case ID_G_SEXTIS:
                                        startGame(&g_tetris, 20, 20, 5);
                                        break;
                                case ID_PAUSE:
                                        pauseGame(!g_tetris.is_paused);
                                        break;
                                case ID_END:
                                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                                        break;
                                case ID_ABOUT:
                                        MessageBox(hwnd, "Tetris\n\nJaroslav Beran, 2015",
                                                "About Tetris", MB_ICONINFORMATION);
                                        break;
                        }
                        break;
                case WM_TIMER:
                        hdc = GetDC(g_hwndMain);
                        ret = t_go(hdc, &g_tetris);
                        ReleaseDC(g_hwndMain, hdc);
                        _stprintf(chText, "Score: %d", g_tetris.score);
                        SetWindowText(g_hwndStatusBar, chText);
                        
                        if (ret == -1) {
                                KillTimer(g_hwndMain, _TimerClock);
                                _stprintf(chText, "Game Over! Your score is %d", g_tetris.score);
                                MessageBox(hwnd, chText, _AppName, MB_OK | MB_ICONINFORMATION);
                        }
                        
                        break;
                case WM_PAINT:
                        onPaint();
                        break;
                case WM_SIZE:
                        SendMessage(g_hwndStatusBar, WM_SIZE, wParam, lParam);
                        break;
                case WM_DESTROY:
                        PostQuitMessage(0);
                        break;
                case WM_CLOSE:
                        pauseGame(TRUE);
                        if (MessageBox(hwnd, "Do you want quit?", _AppName,
                                MB_YESNO | MB_ICONQUESTION) != IDYES) {
                                        return 0;
                                }
                        t_delete_game(&g_tetris);
                        DestroyWindow(hwnd);
                        break;
                default:
                        return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        
        return 0;
}


void startGame(TETRIS_T *tetris, int x_size, int y_size, int brick_size) {
        KillTimer(g_hwndMain, _TimerClock);
        t_delete_game(&g_tetris);
        t_create_game(&g_tetris, x_size, y_size, brick_size);
        InvalidateRect(g_hwndMain, NULL, TRUE);
        g_tetris.is_paused = 0;
        SetTimer(g_hwndMain, _TimerClock, 600, NULL);
}

void pauseGame(BOOL b) {
        if (b == TRUE) {
                if (g_tetris.is_paused)
                        return;
                g_tetris.is_paused = 1;
                KillTimer(g_hwndMain, _TimerClock);
        } else {
                if (!g_tetris.is_paused)
                        return;
                g_tetris.is_paused = 0;
                SetTimer(g_hwndMain, _TimerClock, 600, NULL);
        }
}

void onPaint() {
        PAINTSTRUCT ps;
        HDC hdc;
        RECT rect;
        
        hdc = BeginPaint(g_hwndMain, &ps);
        GetClientRect(g_hwndMain, &rect);
        rect.bottom = rect.bottom - 20;
        g_draw_mesh(hdc, &g_tetris, rect); 
        EndPaint(g_hwndMain, &ps);
}


