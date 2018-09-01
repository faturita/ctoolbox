#include <stdio.h>
#include <stdlib.h>


int yywrap ()
    {
    return 1;
    }

int main()
    {
    yylex();
    return 0;
    }
    
        
