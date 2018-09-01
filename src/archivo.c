#include <stdio.h>
#include <stdlib.h>

void main()
{
char inf='A';
FILE *pf;
pf=fopen("Concurre.c","wb");
if (pf!=NULL)
    {
    printf ("Se ha producido un error.");
    exit (1);
    }
rewind(pf);
while (!feof(pf))
    {
    fread(&inf,sizeof(inf),1,pf);
    printf ("%c",inf);
    }
    

fclose(pf);
}

