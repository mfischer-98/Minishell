/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 12:18:39 by mefische          #+#    #+#             */
/*   Updated: 2026/02/16 12:18:39 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal = 0;

/*	Interactive SIGINT (Ctrl + C) 
	- write new line, cursor at the new line (new start);
	- clear previous prompt with replace_line
	- show prompt again with redisplay
	- status 128 + sigint (2), updating status  */
void	sigint_interactive(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal = 130;
}

/*	Update global variable after signal 
	- Ctrl+C or \ = silent status only
	- 130 sigint and 131 ctrl+\  */
void	sig_exec(int sig)
{
	g_signal = 128 + sig;
}

/*	For child process
	- let command die as usual */
void	sig_default(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*Used in prompt mode
	- Ctrl+C prompt again and Ctrl+\ ignored 
	- restarts readline if interrupted (RESTART)
	- Ctr + C calls sigint interactive 
	- Ctr + \ ignored (SIG_IGN)*/
void	sig_init(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_interactive;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/*Setting up silent status for signals in execve children
	- create sigaction struct
	- RESTART = if system call interrupted retry
	- set ctrl+c = 130
	- add SIGINT and SIGQUIT with same sa 
	- new confif (sa) and dont use old handlers (NULL) */
void	sig_init_exec(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sig_exec;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
