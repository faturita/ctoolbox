#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
char inf='A';
char vec[15];
int cont=0;
FILE *pf,*pf2;
pf=fopen("Condcurre.c","rb");
if (pf==NULL)
    {
    printf ("Se ha producido un error.");
    exit (1);
    }
pf2=fopen("crack.txt","wb+");
if (pf2==NULL)
    {
    printf ("Se ha producido un error.de escritura");
    exit (1);
    }

rewind(pf);
rewind(pf2);
while (!feof(pf))
    {
    fread(&inf,sizeof(inf),1,pf);
    /* Tratamiento del caracter */
    printf ("%c",inf);
    if (!feof(pf)) 
	fwrite(&inf,sizeof(inf),1,pf2);	
    }
    

fclose(pf);
fclose(pf2);
}

