void presentacion()
{
int c,f;
clear();
printw("       #####        #####    ####    ####   ##    ##   ####    ####\n");
printw("       ##   #       ##   #  ##  ##  ##   #  ###  ###  ##  ##  ##   #\n");
printw("       ##   #  ###  #####   ######  ##      ## ## ##  ######  ##\n");
printw("       ##   #       ##      ##  ##  ##   #  ##    ##  ##  ##  ##   #\n");
printw("       #####        ##      ##  ##   ####   ##    ##  ##  ##   ####\n");
printw("                               __---__\n");
printw("                            _-       _--______\n");
printw("                       __--( /     \\ )XXXXXXXXXXXXX_\n");
printw("                     --XXX(   O   O  )XXXXXXXXXXXXXXX-\n");
printw("                    /XXX(       U     )        XXXXXXX\\\n");
printw("                  /XXXXX(              )--_  XXXXXXXXXXX\\\n");
printw("                 /XXXXX/ (      O     )   XXXXXX   \\XXXXX\n");
printw("                 XXXXX/   /            XXXXXX   \\__ \\XXXXX----\n");
printw("                 XXXXXX__/          XXXXXX         \\__----  -\n");
printw("         ---___  XXX__/          XXXXXX      \\__         ---\n");
printw("           --  --__/   ___/\\  XXXXXX            /  ___---=\n");
printw("             -_    ___/    XXXXXX              '--- XXXXXX\n");
printw("               --\\/XXX\\ XXXXXX                      /XXXXX\n");
printw("                 \\XXXXXXXXX                        /XXXXX/\n");
printw("                  \\XXXXXX                        _/XXXXX/\n");
printw("                    \\XXXXX--__/              __-- XXXX/\n");
printw("                     --XXXXXXX---------------  XXXXX--\n");
printw("                        \\XXXXXXXXXXXXXXXXXXXXXXXX-\n");
printw("                          --XXXXXXXXXXXXXXXXXX-\n\n");
printw("                TE GUSTA ASI ARIEL??? AHORA TE VA A GUSTA MAS!!!\n");
getch();
clear();
printw("                               __---__\n");
printw("                            _-       _--______\n");
printw("                       __--(  \\\\  // )XXXXXXXXXXXXX_\n");
printw("                     --XXX(   O   O  )XXXXXXXXXXXXXXX-\n");
printw("                    /XXX(       U     )        XXXXXXX\\\n");
printw("                  /XXXXX(    |     |   )--_  XXXXXXXXXXX\\\n");
printw("                 /XXXXX/ (    |||||   )   XXXXXX   \\XXX ||\n");
printw("                 XXXXX/   /    |||     XXXXXX   \\__ \\XX ||\n");
printw("                 XXXXX  /           XXXXXX         \\__|||||\n");
printw("                 XXXXX /         XXXXXX      \\__          |/\n");
printw("                 XXXXX     \\  XXXXXX            /     ____/\n");
printw("                 XXXXX      \\XXXX              '--- XXXXXX\n");
printw("                 XXXXX       \\                      /XXXXX\n");
printw("                 \\XXXXX\\       \\                   /XXXXX/\n");
printw("                  \\XXXXXX\\    \\ \\                _/XXXXX/\n");
printw("                    \\XXXXX/\\ \\  \\/           __-- XXXX/\n");
printw("                     --X/    /\\/ ------------  XXXXX--\n");
printw("                      /    / XXXXXXXXXXXXXXXXXXXX-\n");
printw("                  /\\/    /--XXXXXXXXXXXXXXXXXX-\n");
printw("                 |    \\/\n");
printw("                 |/___/       TOMAAAA!!!!  ENCIMA DE TROLO... VIRGEN!!!!\n\n");
getch();
attrset(COLOR_PAIR(1)|A_BOLD);
for (c=0;c<80;c++)
    {
    for(f=0;f<25;f++)
	{
	move(f,c-1);
	addch(32);
	move(f,c);
	if ((int) (c%2)) addch(67); else addch(99);
	refresh();
	};
    musicacomiendo1(0);
    }
clear();
}