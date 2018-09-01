#include <stdlib.h>
#include <stdio.h>
#include "logger.h"

int isLogging() {
	if (pfLogFile != NULL)
   	return 1;
   else
   	return 0;
}

void logInfo(char *buffer) {
	if (pfLogFile != NULL ) {
		fputs(buffer, pfLogFile);
   	fflush(pfLogFile);
   }
}


