/*
 * signals.c - The shell's signal handlers
 */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "jobs.h"
#include "signals.h"
#include "tsh.h"
#include "wrappers.h"

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig) {
    int olderrno = errno;
    sigset_t mask, prev;

    Sigfillset(&mask);
    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        deletejob(jobs, (pid_t)pid);
        Sigprocmask(SIG_SETMASK, &prev, NULL);
    }
    if (errno != ECHILD) {
        Sio_error("waitpid error");
    }
    errno = olderrno;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig)
{
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig)
{
    return;
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig)
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}
