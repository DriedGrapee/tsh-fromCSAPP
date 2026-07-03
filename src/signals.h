/*
 * signals.h - The shell's signal handlers
 *
 * Installed by main() in tsh.c. sigchld/sigint/sigtstp are the ones
 * to implement for the lab; sigquit was provided with the handout.
 */
#ifndef SIGNALS_H
#define SIGNALS_H

void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigquit_handler(int sig);

#endif /* SIGNALS_H */
