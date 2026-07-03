/*
 * eval.h - Command-line evaluation
 *
 * eval() is the core of the shell: main() hands it each command line.
 * The rest are its helpers. eval, builtin_cmd, do_bgfg, and waitfg are
 * the functions to implement for the lab; parseline was provided.
 */
#ifndef EVAL_H
#define EVAL_H

#include <sys/types.h>

void eval(char *cmdline);
int parseline(const char *cmdline, char **argv);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

#endif /* EVAL_H */
