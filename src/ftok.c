#include "unpipc.h"

int main(int argc, char **argv)
{
    struct stat stats;
    
    if (argc!=2)
	err_quit ("Usage: ftok <pathname>");
	
    stat(argv[1], &stats);
    
    printf ("st_dev: %lx, st_ino: %lx, key: %x\n",(u_long)stats.st_dev, (u_long) stats.st_ino, ftok(argv[1], 0x57));
    
    exit(0);
    

}

