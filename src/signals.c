/*
 * signals.c - The shell's signal handlers
 */
#include <bits/types/sigset_t.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
 *
 *     The handler's ONLY shared state with the main program is the job
 *     list, always mutated under a full signal block. It deliberately
 *     does not report WHICH child it reaped: one SIGCHLD delivery can
 *     represent several coalesced deaths, so "last pid reaped" is
 *     reap-order-dependent and racy. waitfg() instead watches the job
 *     list for the fg job to disappear (deletejob) or stop (stopjob).
 */
void sigchld_handler(int sig) {
    int olderrno = errno;
    int status;
    sigset_t mask, prev;
    pid_t reaped;

    Sigfillset(&mask);

    while ((reaped = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) { // no hang so that if no child is ready, we don't block
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        if (WIFSIGNALED(status)) { // checks if the child was signaled
            Sio_puts("Job [");
            Sio_putl((long)pid2jid(reaped));
            Sio_puts("] (");
            Sio_putl((long)reaped);
            Sio_puts(") terminated by signal ");
            Sio_putl((long)WTERMSIG(status)); // prints the signal number
            Sio_puts("\n");
            deletejob(jobs, reaped);
        } else if (WIFSTOPPED(status)) {
            Sio_puts("Job [");
            Sio_putl((long)pid2jid(reaped));
            Sio_puts("] (");
            Sio_putl((long)reaped);
            Sio_puts(") stopped by signal ");
            Sio_putl((long)WSTOPSIG(status));
            Sio_puts("\n");
            stopjob(jobs, reaped);
        } else {
            deletejob(jobs, reaped);
        }
        Sigprocmask(SIG_SETMASK, &prev, NULL);
    }
    if (reaped == -1 && errno != ECHILD) { // errno is only meaningful when waitpid returned -1
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
    int olderrno = errno;
    pid_t fg_pid;
    sigset_t mask, prev;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    fg_pid = fgpid(jobs);
    if (fg_pid != 0) {
        kill(-fg_pid, SIGINT);
    } else {
        Sio_puts("killing tsh");
        putchar('\n');
        exit(0);
    }
    Sigprocmask(SIG_SETMASK, &prev, NULL);
    errno = olderrno;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig)
{
    int olderrno = errno;
    pid_t fg_pid;
    sigset_t mask, prev;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    fg_pid = fgpid(jobs);
    if (fg_pid != 0)
        kill(-fg_pid, SIGTSTP);
    else {
        Sio_puts("stopping tsh");
        putchar('\n');
        exit(0);
    }
    Sigprocmask(SIG_SETMASK, &prev, NULL);
    errno = olderrno;
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
