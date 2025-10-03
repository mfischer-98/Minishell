#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdlib.h>
# include "libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int check_exit (char *prompt);
void check_command(char *prompt);
void cat();

#endif