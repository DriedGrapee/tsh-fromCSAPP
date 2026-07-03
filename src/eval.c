/*
 * eval.c - Command-line evaluation
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "jobs.h"
#include "tsh.h"
#include "wrappers.h"

extern char **environ; /* defined in libc */

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char *cmdline) {
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    sigset_t mask, mask_all, prev;

    strncpy(buf, cmdline, MAXLINE);
    bg = parseline(buf, argv);
    if (argv[0] == NULL) {
        return;
    } // ignore empty lines

    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);
    Sigfillset(&mask_all);

    if (!builtin_cmd(argv)) { //built-in command will automatically run the built in command it finds if true
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        if ((pid = Fork()) == 0) {
            Setpgid(0, 0);
            Sigprocmask(SIG_SETMASK, &prev, NULL);
            Execve(argv[0], argv, environ);
        }
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        if (!bg) {
            addjob(jobs, pid, FG, cmdline);
            Sigprocmask(SIG_SETMASK, &mask, NULL);
            int status;
            pid = 0;
            while(!pid) {
                sigsuspend(&prev);
            }
            Sigprocmask(SIG_SETMASK, &prev, NULL);
        } else {
            addjob(jobs, pid, BG, cmdline);
            Sigprocmask(SIG_SETMASK, &prev, NULL);
            printf("%d %s", pid, cmdline);
        }
    }
    return;
}

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv)
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    }
    else {
        delim = strchr(buf, ' ');
    }

    while (delim) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;

        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        }
        else {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	    argv[--argc] = NULL;
    }
    return bg;
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv) {
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }
    if (!strcmp(argv[0], "jobs")) {
        listjobs(jobs);
        return(1);
    }
    if (!strcmp(argv[0], "bg")) {
        do_bgfg(argv);
        return(1);
    }
    if (!strcmp(argv[0], "fg")) {
        do_bgfg(argv);
        return(1);
    }
    if (!strcmp(argv[0], "&")) {
        return 1;
    }
    return 0;     /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
    return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    return;
}
