#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "Sonido.h"
#include <signal.h>
#include "Colores.h"


void sigint_handler()
{
signal(SIGTERM,SIG_IGN);
signal(SIGTERM,sigint_handler);
}


void imprimir(opcion1,opcion2,opcion3,opcion4,opcion5,contdin)
char opcion1[20];
char opcion2[20];
char opcion3[20];
char opcion4[20];
char opcion5[20];
int contdin;
{int yy=8,i;
if (strcmp(opcion1,"0"))
    {move(yy,40-((strlen(opcion1))/2));
    for (i=0;i<strlen(opcion1);i++)
	{
	if (contdin==1)
	    addch (opcion1[i]+21*256);
	else
	    addch (A_DIM | opcion1[i]);
	refresh();}
    yy+=2;
    }
if (strcmp(opcion2,"0"))
    {move(yy,40-((strlen(opcion2))/2));
    for (i=0;i<strlen(opcion2);i++)
	{
	if (contdin==2)
	    addch (opcion2[i]+21*256);
	else
	    addch (A_DIM | opcion2[i]);
	refresh();}
    yy+=2;
    }
if (strcmp(opcion3,"0"))
    {move(yy,40-((strlen(opcion3))/2));
    for (i=0;i<strlen(opcion3);i++)
	{
	if (contdin==3)
	    addch (opcion3[i]+21*256);
	else
	    addch (A_DIM | opcion3[i]);
	refresh();}
    yy+=2;
    }
if (strcmp(opcion4,"0"))
    {move(yy,40-((strlen(opcion4))/2));
    for (i=0;i<strlen(opcion4);i++)
	{
	if (contdin==4)
	    addch (opcion4[i]+21*256);
	else
	    addch (A_DIM | opcion4[i]);
	refresh();}
    yy+=2;
    }
if (strcmp(opcion5,"0"))
    {move(yy,40-((strlen(opcion5))/2));
    for (i=0;i<strlen(opcion5);i++)
	{
	if (contdin==5)
	    addch (A_DIM | opcion5[i]+21*256);
	else
	    addch (opcion5[i]);
	refresh();}
    yy+=2;
    }
}

int menu (titulo,opcion1,opcion2,opcion3,opcion4,opcion5)
char titulo[30];
char opcion1[20];
char opcion2[20];
char opcion3[20];
char opcion4[20];
char opcion5[20];
{
int yy=8,op,i,cont=0,contdin=1;
chtype c;
move(6,40-((strlen(titulo))/2));
for (i=0;i<strlen(titulo);i++)
    {usleep(30000);
    addch (A_BOLD | titulo[i]+22*256);
    refresh();
    musicacomiendo(0);
}
if (strcmp(opcion1,"0"))
    {move(yy,40-((strlen(opcion1))/2));
    for (i=0;i<strlen(opcion1);i++)
	{usleep(30000);
	addch (opcion1[i]+21*256);
	refresh();
	musicacomiendo(0);
}
    yy+=2;
    cont++;}
if (strcmp(opcion2,"0"))
    {move(yy,40-((strlen(opcion2))/2));
    for (i=0;i<strlen(opcion2);i++)
	{usleep(30000);
	addch (A_DIM | opcion2[i]);
	refresh();
	musicacomiendo(0);
}
    yy+=2;
    cont++;}
if (strcmp(opcion3,"0"))
    {move(yy,40-((strlen(opcion3))/2));
    for (i=0;i<strlen(opcion3);i++)
	{usleep(30000);
	addch (A_DIM | opcion3[i]);
	refresh();
	musicacomiendo(0);
}
    yy+=2;
    cont++;}
if (strcmp(opcion4,"0"))
    {move(yy,40-((strlen(opcion4))/2));
    for (i=0;i<strlen(opcion4);i++)
	{usleep(30000);
	addch (A_DIM | opcion4[i]);
	refresh();
	musicacomiendo(0);
}
    yy+=2;
    cont++;}
if (strcmp(opcion5,"0"))
    {move(yy,40-((strlen(opcion5))/2));
    for (i=0;i<strlen(opcion5);i++)
	{usleep(30000);
	addch (A_DIM | opcion5[i]);
	refresh();
	musicacomiendo(0);
}
    yy+=2;
    cont++;}
c=getch();
while (c!=10)
    {if (c==KEY_DOWN)
	contdin++;
    if (c==KEY_UP)
	contdin--;
    if (contdin==0)
	contdin=cont;
    if (contdin>cont)
	contdin=1;
    imprimir(opcion1,opcion2,opcion3,opcion4,opcion5,contdin);
    c=getch();
    }	
return contdin;
}

void main()
{
int control4;
char comando[15];
sigint_handler();
initscr();
start_color();
inicializarcolores();
curs_set(0);
noecho();  
keypad(stdscr,TRUE);
strcpy(comando,"clear");
control4=menu("MANEJO DE JUEGO","Teclado","Joystick ","0","0","0");
	    switch(control4)
	    {
	    case 1:
		strcpy(comando,"paccli darkstar diego 1");break;
	    case 2:
		strcpy(comando,"paccli darkstar diego 3");break;
	    }
clear();
system(comando);    
endwin();
curs_set(1);
clear();
}

