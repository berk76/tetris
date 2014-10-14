/*
*	multi.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	12.10.2014
*	redistributable under the terms of the GNU/GPLv3
*/



#if	!defined(__MULTI_H)
#define __MULTI_H

/*
*******************************************************************
* Minimalni sada funkci pro implementaci TETRISu                  *
*******************************************************************
*/

/*
* init_graph() inicializuje graficke rozhrani pokud je to nutne.
* V pripade ze na dane platforme inicializace neni nutna, tak se ponecha
* prazdna implementace.
* Navratova hodnota 0 = vse ok, v opacnem pripade kod chyby
*
*/
extern int m_init_graph(void);

/*
* close_graph() uvolnuje a rusi rozhrani, pokud je to nutne.
*/
extern void m_close_graph(void);

/*
* Vrati maximalni horizontalni rozmer obrazovky.
*/
extern int m_get_max_x(void);

/*
* Vrati maximalni vertikalni rozmer obrazovky.
*/
extern int m_get_max_y(void);

/*
* Definice barevne palety pro 16 barev
*/
enum M_COLORS {
    M_BLACK		= 0,
    M_BLUE		= 1,
    M_GREEN		= 2,
    M_CYAN		= 3,
    M_RED		= 4,
    M_MAGENTA		= 5,
    M_BROWN		= 6,
    M_LIGHTGRAY		= 7,
    M_DARKGRAY		= 8,
    M_LIGHTBLUE		= 9,
    M_LIGHTGREEN	= 10,
    M_LIGHTCYAN		= 11,
    M_LIGHTRED		= 12,
    M_LIGHTMAGENTA 	= 13,
    M_YELLOW		= 14,
    M_WHITE		= 15
};

/*
* Nastaveni barvy pera
*/
extern void m_setcolor(int color);

/*
* Nastaveni barvy papiru
*/
extern void m_setbkcolor(int color);

/*
* Vykresleni bodu na obrazovku
*/
extern void m_putpixel(int x, int y, int color);

/*
* Ziskani barvy bodu obrazovky
*/
extern int m_getpixel(int x, int y);


/*
* Vymazani obrazovky
*/
extern void m_clear_screen(void);

/*
* Test zda byla zmacknuta klavesa
* 0 = klavesa zmacknuta nebyla, jinak ASCII kod klavesy
*/
extern int m_kbhit(void);

/*
* Uspani na n milisekund
*/
extern void m_delay(unsigned milliseconds);

/*
*******************************************************************
* Nasleduje nekolik funkci, ktere mohou byt implementovany na     *
* zaklade predchozich. Nicmene nekdy muze byt efektivnejsi jejich *
* nativni implementace na hostitelskem prostredi.                 *
*******************************************************************
*/

/*
* Nastaveni velikosti fontu
*/
extern void m_settextsize(int charsize);

/*
* Ziskani velikosti fontu
*/
extern int m_gettextsize(void);

/*
* Vypis textu na obrazovku
*/
extern void m_outtextxy(int x, int y, char *s);

/*
* Vykresleni obdelniku
*/
extern void m_rectangle(int left, int top, int right, int bottom);

/*
* Vybarveni obdelniku
*/
extern void m_fill_rect(int x, int y, int width, int height, int color);

/*
* Ziskani velikosti vystrizku obrazovky
*/
extern unsigned m_imagesize(int left, int top, int right, int bottom);

/*
* Ziskani vystrizku obrazovky
*/
extern void m_getimage(int left, int top, int right, int bottom, void *bitmap);

/*
* Vykresleni vystrizku obrazovku
*/
extern void m_putimage(int left, int top, void *bitmap);

#endif

