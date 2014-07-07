/*
*	guicntls.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/


#ifndef _GUICNTLS_
#define _GUICNTLS_

typedef struct {
	int x;
	int y;
	int width;
	int height;
	void *buff;
} GUI_CNTL;


extern GUI_CNTL *gui_show_message(char *msg);
extern void gui_delete_message(GUI_CNTL *cntl);
extern int gui_wait_for_key(char *s);
extern int gui_wait_for_any_key();

#endif

