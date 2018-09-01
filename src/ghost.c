#include "./lib/daemon.h"

int main(int argc, char **argv) {
    printf ("La mar estaba serena, serena estaba la mar...");
    
    daemon_init(argv[0],LOG_INFO | LOG_LOCAL2);
    
    while (1) {
	syslog( LOG_INFO | LOG_LOCAL2, "lalal");
    }

}