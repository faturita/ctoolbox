#ifndef __unpipc_h
#define __unpipc_h

//nclude "../config.h"

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <unistd.h>
#include <sys/wait.h>

#ifdef HAVE_MQUEUE_H
#include <mqueue.h>
#endif

#ifdef HAVE_SEMAPHORE_H
#include <semaphore.h>
#endif

// check page 506 unix network programming vol 2 (Stevens, PH)
#endif


#include <stdarg.h>
#include <syslog.h>

int daemon_proc;

static void err_doit(int, int, const char *, va_list);

void err_quit(const char*fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int errno_save, n;
    
    errno_save = errno;
    exit(0);
}
