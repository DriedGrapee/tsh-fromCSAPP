/*
 * tsh.h - Shared constants and global shell state
 *
 * Every module includes this header. It holds the manifest constants
 * and the few globals that are shared between the eval loop and the
 * signal handlers.
 */
#ifndef TSH_H
#define TSH_H

#include <signal.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */

/* Global shell state (defined in tsh.c) */
extern int verbose;               /* if true, print additional output */
extern char sbuf[MAXLINE];        /* for composing sprintf messages */
extern volatile sig_atomic_t pid; /* set by eval's fork, cleared/updated by
                                     sigchld_handler while waiting for fg job */

#endif /* TSH_H */
