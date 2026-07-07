/*
 * wrappers.h - csapp-style error handling and Unix syscall wrappers
 *
 * Capitalized functions are error-checking wrappers around the
 * same-named syscall, drawn from csapp.c (see reference/csapp.c).
 * They report the error and exit, so call sites stay clean.
 */
#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <signal.h>
#include <sys/types.h>

/* Error-handling functions */
void unix_error(char *msg);
void app_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);

/* Async-signal-safe I/O (safe to call from signal handlers) */
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void Sio_error(char s[]);
ssize_t Sio_puts(char s[]);
ssize_t Sio_putl(long v);

/* Wrappers for Unix process control functions */
pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
void Pause(void);
unsigned int Sleep(unsigned int secs);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp(void);

/* Wrappers for Unix signal functions */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
int Sigismember(const sigset_t *set, int signum);
int Sigsuspend(const sigset_t *set);

#endif /* WRAPPERS_H */
