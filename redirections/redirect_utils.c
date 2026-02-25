/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 16:28:20 by mefische          #+#    #+#             */
/*   Updated: 2026/02/18 16:28:20 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Check if commandline has redirects */
int	has_redirect(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_IN || temp->type == NODE_OUT
			|| temp->type == NODE_APPEND || temp->type == NODE_HERE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

/* Helper to run a builtin with redirections in the parent process
	- save terminal → point stdout to file → run builtin → restore terminal
	- save keyboard (oldstdin) and save terminal (oldstdout)
	- restore fds */
void	run_builtin_redirects(char **commandline, t_mshell_data *data)
{
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(0);
	old_stdout = dup(1);
	if (old_stdin < 0 || old_stdout < 0)
		return (perror("dup"), (void)0);
	if (apply_redirects(data->tokens) == 0)
		run_command(commandline, data);
	dup2(old_stdin, 0);
	dup2(old_stdout, 1);
	close(old_stdin);
	close(old_stdout);
}

/* If it is builtin, i need to do an extra step with dup2,
	if it is not, it can go directly to ft_execve function */
int	is_builtin(char **commands)
{
	int	i;
	
	i = 0;
	while(commands[i])
	{
		if (!ft_strcmp(commands[0], "pwd"))
			return (1);
		else if (!ft_strcmp(commands[0], "cd"))
			return (1);
		else if (!ft_strcmp(commands[0], "env"))
			return (1);
		else if (!ft_strcmp(commands[0], "echo"))
			return (1);
		else if (!ft_strcmp(commands[0], "export"))
			return (1);
		else if (!ft_strcmp(commands[0], "unset"))
			return (1);
		else if (!ft_strcmp(commands[0], "exit"))
			return (1);
		i++;
	}
	return(0);
}
