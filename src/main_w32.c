/*
*       main_w32.c
*       
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 10.3.2015        
*/


#include <windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include "tetris.h"
#include "main.h"
#include "resource.h"

//#define TOUCH_SUPPORT


#define _MainClassName TEXT("WinAPIMainClass")
#define _AppName TEXT("Tetris")
#define _TimerClock 1
#define _TimerInterval 300

HINSTANCE g_hInstance;
HWND g_hwndMain;
HWND g_hwndStatusBar;
MSG msg;
TETRIS_T g_tetris;
HDC g_hdc;        /* Only for m_put_mesh_pixel */

typedef struct {
        HPEN hPen;
        HBRUSH hBrush;
} COLOR_T;

COLOR_T colors[T_COLORS_SIZE];

static BOOL InitApp();
static BOOL DeleteApp();
static LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void startGame(TETRIS_T *tetris, int x_size, int y_size, int brick_size);
static void pauseGame(BOOL b);
static void update_score();
static void onPaint();
#ifdef TOUCH_SUPPORT
static BOOL on_gesture(WPARAM wParam, LPARAM lParam);
#endif
static void g_draw_mesh(TETRIS_T *tetris, RECT client);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow) {
        g_hInstance = hInstance;
        
        InitCommonControls();
        if (!InitApp())
                return FALSE;
                
        while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
        
        DeleteApp();
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
        
        g_hwndMain = CreateWindowEx(0,     
                _MainClassName, 
                _AppName, 
                WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                CW_USEDEFAULT, CW_USEDEFAULT, 
                300, 600, 
                (HWND)NULL, 
                LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MAINMENU)), 
                g_hInstance, 
                NULL); 
                
        if (g_hwndMain == NULL)
                return FALSE;
                
                
        g_hwndStatusBar = CreateWindowEx(0,
                STATUSCLASSNAME,
                TEXT("Score:"),
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
        
        
        #ifdef TOUCH_SUPPORT
        
        // set up our want / block settings
        DWORD dwPanWant  = GC_PAN_WITH_SINGLE_FINGER_VERTICALLY | GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY;
        DWORD dwPanBlock = GC_PAN_WITH_GUTTER | GC_PAN_WITH_INERTIA;
        
        // set the settings in the gesture configuration
        GESTURECONFIG gc[] = {{ GID_ZOOM, GC_ZOOM, 0 },
                              { GID_ROTATE, GC_ROTATE, 0},
                              { GID_PAN, dwPanWant , dwPanBlock}                     
                             };    
                             
        UINT uiGcs = 3;
        SetGestureConfig(g_hwndMain, 0, uiGcs, gc, sizeof(GESTURECONFIG));  
        
        #endif
        
        colors[0].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0xFF0000);
        colors[0].hBrush = CreateSolidBrush(0xFF0000);
        colors[1].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0x00FF00);
        colors[1].hBrush = CreateSolidBrush(0x00FF00);
        colors[2].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0xFFFF00);
        colors[2].hBrush = CreateSolidBrush(0xFFFF00);
        colors[3].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0x0000FF);
        colors[3].hBrush = CreateSolidBrush(0x0000FF);
        colors[4].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0xFF00FF);
        colors[4].hBrush = CreateSolidBrush(0xFF00FF);
        colors[5].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0x00FFFF);
        colors[5].hBrush = CreateSolidBrush(0x00FFFF);
        colors[6].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0xFFFFFF);
        colors[6].hBrush = CreateSolidBrush(0xFFFFFF);
        colors[7].hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, 0x000000);
        colors[7].hBrush = CreateSolidBrush(0x000000);
        
        t_create_game(&g_tetris, 10, 20, 4);
                        
        ShowWindow(g_hwndMain, SW_SHOWNORMAL);
        UpdateWindow(g_hwndMain);
                                  
        return TRUE;
}


BOOL DeleteApp() {
        DeleteObject(colors[0].hPen);
        DeleteObject(colors[0].hBrush);
        DeleteObject(colors[1].hPen);
        DeleteObject(colors[1].hBrush);
        DeleteObject(colors[2].hPen);
        DeleteObject(colors[2].hBrush);
        DeleteObject(colors[3].hPen);
        DeleteObject(colors[3].hBrush);
        DeleteObject(colors[4].hPen);
        DeleteObject(colors[4].hBrush);
        DeleteObject(colors[5].hPen);
        DeleteObject(colors[5].hBrush);
        DeleteObject(colors[6].hPen);
        DeleteObject(colors[6].hBrush);
        DeleteObject(colors[7].hPen);
        DeleteObject(colors[7].hBrush);
        
        return TRUE;
}


LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        TCHAR chText[100];
        int ret;
        
        switch (uMsg) {
                case WM_KEYDOWN:
                        g_hdc = GetDC(g_hwndMain);
                        switch (wParam) {        
                                case VK_UP:
                                        pauseGame(FALSE);
                                        t_rotate(&g_tetris, 1);
                                        break;
                                case VK_DOWN:
                                        pauseGame(FALSE);
                                        while(t_move_down(&g_tetris) != -1)
                                                ;
                                        break;
                                case VK_LEFT:
                                        pauseGame(FALSE);
                                        t_move_left(&g_tetris);
                                        break;
                                case VK_RIGHT:
                                        pauseGame(FALSE);
                                        t_move_right(&g_tetris);
                                        break;
                                case 'p':
                                        pauseGame(!g_tetris.is_paused);
                                        break;
                                case 'P':
                                        pauseGame(!g_tetris.is_paused);
                                        break;                        
                        }
                        ReleaseDC(g_hwndMain, g_hdc);
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
                                        startGame(&g_tetris, 20, 20, 6);
                                        break;
                                case ID_PAUSE:
                                        pauseGame(!g_tetris.is_paused);
                                        break;
                                case ID_END:
                                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                                        break;
                                case ID_ABOUT:
                                        _stprintf(chText, "Tetris %s\nhttps://bitbucket.org/berk76/tetris\n(c) 2015 Jaroslav Beran", TETRIS_VERSION);
                                        MessageBox(hwnd, chText,
                                                "About Tetris", MB_ICONINFORMATION);
                                        break;
                        }
                        break;
                case WM_TIMER:
                        g_hdc = GetDC(g_hwndMain);
                        ret = t_go(&g_tetris);
                        ReleaseDC(g_hwndMain, g_hdc);
                        update_score();
                        
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
                #ifdef TOUCH_SUPPORT
                case WM_GESTURE:
                        if (on_gesture(wParam, lParam))
                                break;  
                #endif        
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
        SetTimer(g_hwndMain, _TimerClock, _TimerInterval, NULL);
}

void pauseGame(BOOL b) {
        if (b == TRUE) {
                if (g_tetris.is_paused)
                        return;
                g_tetris.is_paused = 1;
                KillTimer(g_hwndMain, _TimerClock);
                SetWindowText(g_hwndStatusBar, TEXT("Paused"));
        } else {
                if (!g_tetris.is_paused)
                        return;
                g_tetris.is_paused = 0;
                SetTimer(g_hwndMain, _TimerClock, _TimerInterval, NULL);
                update_score();
        }
}

void update_score() {
        TCHAR chText[100];
        _stprintf(chText, "Score: %d", g_tetris.score);
        SetWindowText(g_hwndStatusBar, chText);
}

void onPaint() {
        PAINTSTRUCT ps;
        RECT rect;
        
        g_hdc = BeginPaint(g_hwndMain, &ps);
        GetClientRect(g_hwndMain, &rect);
        rect.bottom = rect.bottom - 20;
        g_draw_mesh(&g_tetris, rect); 
        EndPaint(g_hwndMain, &ps);
}

#ifdef TOUCH_SUPPORT
BOOL on_gesture(WPARAM wParam, LPARAM lParam) {
        GESTUREINFO gi;
        HDC hdc;
        static POINT m_first;
        POINT m_last;
        ZeroMemory(&gi, sizeof(GESTUREINFO));
        gi.cbSize = sizeof(GESTUREINFO);
        BOOL bResult  = GetGestureInfo((HGESTUREINFO)lParam, &gi);
        BOOL bHandled = FALSE;
        /* TCHAR chText[100]; */
        
        g_hdc = GetDC(g_hwndMain);
        if (bResult) {
                switch (gi.dwID){
                        case GID_PAN:
                                if (gi.dwFlags & GF_BEGIN) {
                                        m_first.x = gi.ptsLocation.x;
                                        m_first.y = gi.ptsLocation.y;     
                                } else {
                                        m_last.x = gi.ptsLocation.x;
                                        m_last.y = gi.ptsLocation.y;
                                        if (abs(m_last.x - m_first.x) > abs(m_last.y - m_first.y)) {
                                                if ((gi.dwFlags & GF_END) || (abs(m_last.x - m_first.x) > g_tetris.element_size)) {
                                                        if ((m_last.x - m_first.x) > 0) {
                                                                m_first.x += g_tetris.element_size;
                                                                m_first.y = m_last.y; 
                                                                pauseGame(FALSE);
                                                                t_move_right(&g_tetris);
                                                        } else {
                                                                m_first.x -= g_tetris.element_size;
                                                                m_first.y = m_last.y;
                                                                pauseGame(FALSE);
                                                                t_move_left(&g_tetris);
                                                        }
                                                }
                                        } else {
                                                if (gi.dwFlags & GF_END) { 
                                                        if ((m_last.y - m_first.y) > 0) {
                                                                pauseGame(FALSE);
                                                                while(t_move_down(&g_tetris) != -1)
                                                                ;
                                                        } else {
                                                                pauseGame(FALSE);
                                                                t_rotate(&g_tetris, 1);
                                                        }
                                                }
                                        }     
                                }
                                bHandled = TRUE;
                                break;
                        case GID_ROTATE:
                                if (gi.dwFlags & GF_BEGIN) {
                                        m_first.x = (long) 180 * GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments) / M_PI; 
                                }
                                if (gi.dwFlags & GF_END) {
                                        m_last.x = (long) 180 * GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments) / M_PI;
                                        /*
                                        _stprintf(chText, "Begin=%ld End=%ld", m_first.x, m_last.x);
                                        MessageBox(g_hwndMain, chText, _AppName, MB_OK | MB_ICONINFORMATION);
                                        */
                                        int i;
                                        for (i = 0; i < abs(m_last.x / 90); i++)
                                                t_rotate(&g_tetris, (m_last.x > 1) ? 1 : -1);
                                }
                                bHandled = TRUE;
                                break;
                }
                CloseGestureInfoHandle((HGESTUREINFO)lParam);          
        }
        
        ReleaseDC(g_hwndMain, g_hdc);
        return bHandled; 
}
#endif


void g_draw_mesh(TETRIS_T *tetris, RECT client) {
        int i1, i2;
        int margin;
        
        margin = 15;
        i1 = (client.right - client.left - margin) / tetris->grid_size_x;
        i2 = (client.bottom - client.top - margin) / tetris->grid_size_y;
        tetris->element_size = (i1 < i2) ? i1 : i2;
        tetris->origin_x = ((client.right - client.left) - (tetris->grid_size_x * tetris->element_size)) / 2;
        tetris->origin_y = ((client.bottom - client.top) - (tetris->grid_size_y * tetris->element_size)) / 2;
        
        for (i1 = 0; i1 < tetris->grid_size_x; i1++) {
                for (i2 = 0; i2 < tetris->grid_size_y; i2++)
                        m_put_mesh_pixel(tetris, i1, i2, tetris->grid_map[i1][i2]);
        }
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        SelectObject(g_hdc, colors[color].hPen);
        SelectObject(g_hdc, colors[color].hBrush);
        Rectangle(g_hdc, 
                tetris->origin_x + x * tetris->element_size, 
                tetris->origin_y + y * tetris->element_size, 
                tetris->origin_x + x * tetris->element_size + tetris->element_size, 
                tetris->origin_y + y * tetris->element_size + tetris->element_size);
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        m_put_mesh_pixel(tetris, x, y, TETRIS_BK_COLOR);
}
