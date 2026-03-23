/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:13 by mefische          #+#    #+#             */
/*   Updated: 2026/03/23 10:40:56 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Handles a single command (no pipes)
	- builds a string array from tokens, then checks:
	- redirect-only input (>, >>, <) → redirect_start
	- no redirects → run_command (builtins or external)
	- has redirects + builtin → run_builtin_redirects
	- has redirects + external → ft_execve */
static void	execute_single_command(t_mshell_data *data)
{
	char	**commands;

	commands = array_join(data->tokens);
	if (!commands)
		return ;
	if (!ft_strcmp(data->tokens->input, ">") || !ft_strcmp(data->tokens->input,
			">>") || !ft_strcmp(data->tokens->input, "<"))
	{
		free_array(commands, array_size(commands));
		return (redirect_start(data->tokens, data));
	}
	if (commands && commands[0] && data->tokens->type == NODE_WORD)
	{
		if (!has_redirect(data->tokens))
			run_command(commands, data);
		else if (is_builtin(commands))
			run_builtin_redirects(commands, data);
		else
			ft_execve(commands, data);
	}
	free_array(commands, array_size(commands));
}

/* Forks a child to run an external command (non-builtin)
	- Child resets signals to default and calls execute_external_command.
	- Parent waits and captures exit status, WIFEXITED for normal exit
	- WIFSIGNALED for signal death (128 + signal number) */
void	ft_execve(char **commandline, t_mshell_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), data->exit_status = 1, (void)0);
	if (pid == 0)
	{
		sig_default(SIGINT);
		sig_default(SIGQUIT);
		execute_external_command(commandline, data, data->tokens);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
}

void	run_command(char **commandline, t_mshell_data *data)
{
	if (!commandline || !commandline[0] || commandline[0][0] == '\0')
	{
		data->exit_status = 0;
		return ;
	}
	if (!ft_strcmp(commandline[0], "pwd"))
		data->exit_status = pwd();
	else if (!ft_strcmp(commandline[0], "cd"))
		data->exit_status = cd(data, commandline);
	else if (!ft_strcmp(commandline[0], "env"))
		data->exit_status = env(commandline, data);
	else if (!ft_strcmp(commandline[0], "echo"))
		data->exit_status = echo(commandline);
	else if (!ft_strcmp(commandline[0], "export"))
		data->exit_status = export(commandline, data);
	else if (!ft_strcmp(commandline[0], "unset"))
		data->exit_status = unset(commandline, data);
	else if (!ft_strcmp(commandline[0], "exit"))
		check_exit(commandline, data);
	else
		ft_execve(commandline, data);
}

/* Walks the token list and removes any token with null or empty input
	- needed after expansion, where $EMPTY becomes "" and must be skipped
	- before execution to avoid passing empty tokens */
static void	skip_empty_tokens(t_tokens **tokens)
{
	t_tokens	*current;
	t_tokens	*prev;
	t_tokens	*next;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (!current->input || current->input[0] == '\0')
		{
			if (prev)
				prev->next = next;
			else
				*tokens = next;
			free(current->input);
			free(current);
		}
		else
			prev = current;
		current = next;
	}
}

/* EXECUTOR: expands variables, skips empty tokens, preps heredocs
	- if any pipe is found goes to piped execution
	- otherwise runs a single command */
void	executor(t_mshell_data *data)
{
	t_tokens	*temp;

	if ((data->tokens->input[0] == '\"' && data->tokens->input[1] == '\"')
		|| !data->tokens->input)
	{
		data->exit_status = 127;
		return (print_cmd_not_found("\'\'"));
	}
	expand_all_tokens(data);
	skip_empty_tokens(&data->tokens);
	if (!data->tokens)
	{
		data->exit_status = 0;
		return ;
	}
	if (!prep_heredoc(data))
		return ;
	temp = data->tokens;
	while (temp)
	{
		if (temp->type == NODE_PIPE)
			return (execute_piped_commands(data, data->tokens), (void)0);
		temp = temp->next;
	}
	execute_single_command(data);
}
