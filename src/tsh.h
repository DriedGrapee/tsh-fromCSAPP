/*
 * tsh.h - Shared constants and global shell state
 *
 * Every module includes this header. It holds the manifest constants
 * and the few globals that are shared between the eval loop and the
 * signal handlers.
 */
#ifndef TSH_H
#define TSH_H

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */

/* Global shell state (defined in tsh.c).
 * Note: there is deliberately no shared handler<->main pid variable;
 * signal handlers communicate with the main program only through the
 * job list (see jobs.h), always mutated with signals blocked. */
extern int verbose;               /* if true, print additional output */
extern char sbuf[MAXLINE];        /* for composing sprintf messages */

#endif /* TSH_H */
