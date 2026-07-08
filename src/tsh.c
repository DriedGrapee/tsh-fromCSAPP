/*
 * tsh - A tiny shell program with job control
 *
 * <Put your name and login ID here>
 *
 * Program flow:
 *   main() parses the shell's own options, installs the signal
 *   handlers (signals.c), initializes the job list (jobs.c), and then
 *   loops forever reading a command line and handing it to eval()
 *   (eval.c). Error-checked syscall wrappers live in wrappers.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "eval.h"
#include "jobs.h"
#include "signals.h"
#include "tsh.h"
#include "wrappers.h"

/* Global variables */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
char sbuf[MAXLINE];         /* for composing sprintf messages */
/* End global variables */

static void usage(void);

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler);

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {
    	/* Read command line */
    	if (emit_prompt) {
    	    printf("%s", prompt);
    	    fflush(stdout);
    	}
    	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
    	    app_error("fgets error");
    	if (feof(stdin)) { /* End of file (ctrl-d) */
    	    fflush(stdout);
    	    exit(0);
    	}

    	/* Evaluate the command line */
    	eval(cmdline);
    	fflush(stdout);
    	fflush(stdout);
    }

    exit(0); /* control never reaches here */
}

/*
 * usage - print a help message
 */
static void usage(void)
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}
