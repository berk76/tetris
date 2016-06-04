/*
*	main.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	10.3.2015
*	redistributable under the terms of the GNU/GPLv3
*/

#include <windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include "tetris.h"
#include "graph.h"
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

static BOOL InitApp();
static LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void startGame(TETRIS_T *tetris, int x_size, int y_size, int brick_size);
static void pauseGame(BOOL b);
static void update_score();
static void onPaint();
#ifdef TOUCH_SUPPORT
static BOOL on_gesture(WPARAM wParam, LPARAM lParam);
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow) {
        g_hInstance = hInstance;
        
        InitCommonControls();
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
                CW_USEDEFAULT, CW_USEDEFAULT, // souøadnice na obraziovce
                300, 600, // rozmìry - šíøka a výška
                (HWND)NULL, // okna pøedka
                LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MAINMENU)), // handle hlavní nabídky
                g_hInstance, // handle instance
                NULL); // další "uživatelská" data
                
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
        
        t_create_game(&g_tetris, 10, 20, 4);                
        ShowWindow(g_hwndMain, SW_SHOWNORMAL);
        UpdateWindow(g_hwndMain);
        
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
                                        t_rotate(hdc, &g_tetris, 1);
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
                                case 'p':
                                        pauseGame(!g_tetris.is_paused);
                                        break;
                                case 'P':
                                        pauseGame(!g_tetris.is_paused);
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
                                        startGame(&g_tetris, 20, 20, 6);
                                        break;
                                case ID_PAUSE:
                                        pauseGame(!g_tetris.is_paused);
                                        break;
                                case ID_END:
                                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                                        break;
                                case ID_ABOUT:
                                        MessageBox(hwnd, "Tetris\nhttps://bitbucket.org/berk76/tetris\n(c) 2015 Jaroslav Beran",
                                                "About Tetris", MB_ICONINFORMATION);
                                        break;
                        }
                        break;
                case WM_TIMER:
                        hdc = GetDC(g_hwndMain);
                        ret = t_go(hdc, &g_tetris);
                        ReleaseDC(g_hwndMain, hdc);
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
        HDC hdc;
        RECT rect;
        
        hdc = BeginPaint(g_hwndMain, &ps);
        GetClientRect(g_hwndMain, &rect);
        rect.bottom = rect.bottom - 20;
        g_draw_mesh(hdc, &g_tetris, rect); 
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
        
        hdc = GetDC(g_hwndMain);
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
                                                                t_move_right(hdc, &g_tetris);
                                                        } else {
                                                                m_first.x -= g_tetris.element_size;
                                                                m_first.y = m_last.y;
                                                                pauseGame(FALSE);
                                                                t_move_left(hdc, &g_tetris);
                                                        }
                                                }
                                        } else {
                                                if (gi.dwFlags & GF_END) { 
                                                        if ((m_last.y - m_first.y) > 0) {
                                                                pauseGame(FALSE);
                                                                while(t_move_down(hdc, &g_tetris) != -1)
                                                                ;
                                                        } else {
                                                                pauseGame(FALSE);
                                                                t_rotate(hdc, &g_tetris, 1);
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
                                                t_rotate(hdc, &g_tetris, (m_last.x > 1) ? 1 : -1);
                                }
                                bHandled = TRUE;
                                break;
                }
                CloseGestureInfoHandle((HGESTUREINFO)lParam);          
        }
        
        ReleaseDC(g_hwndMain, hdc);
        return bHandled; 
}
#endif


