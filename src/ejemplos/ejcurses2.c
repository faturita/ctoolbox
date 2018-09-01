
#include <stdio.h>
/* Librerias de funciones de control de pantalla y teclado */
/* Para verlas en el manual haga man curs_(cualquier funcion) */


#include <ncurses/curses.h>

void main()
{
char c;
initscr();
start_color();
COLOR_PAIR(8);
for (c=-120;c<120;c++) {addch(200);printf ("\n");refresh();getch();}
endwin();
exit(0);


/*WINDOW *w;
initscr();
start_color();
w=newwin(1,1,1,1);
wmove(w,1,1);
waddch(w,'a');
wrefresh(w);
getch();
mvwin(w,10,10);
wrefresh(w);
refresh();
getch();
mvwin(w,15,15);
wrefresh(w);
getch();
mvwin(w,2,2);
wrefresh(w);
getch();
endwin();*/
}



