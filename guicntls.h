/*
*	guicntls.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/


#ifndef _GUICNTLS_
#define _GUICNTLS_

extern void gui_message(char *msg);
extern int gui_confirm(char *msg);
extern int gui_option(char *msg, char *options);

#endif

