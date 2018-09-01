
#include <stdio.h>
/* Librerias de funciones de control de pantalla y teclado */
/* Para verlas en el manual haga man curs_(cualquier funcion) */


#include <ncurses/curses.h>

void main()
{
int Ppid=getpid();
WINDOW *w;
initscr();

fork();
if (Ppid==getppid())
    {
    while (1)
    {
    w=newwin(1,1,1,1);
    wmove(w,1,1);
    waddch(w,'a');
    wrefresh(w);
    mvwin(w,10,10);
    wrefresh(w);
    move(1,1);addch(' ');
    }
    }
    
else
    {
    



while (1)
    {
w=newwin(1,1,10,10);
wmove(w,1,1);
waddch(w,'b');
wrefresh(w);
getch();
mvwin(w,10,10);
wrefresh(w);
getch();
    }

    }
    



endwin();
}



