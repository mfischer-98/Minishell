/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:13 by mefische          #+#    #+#             */
/*   Updated: 2026/03/02 14:48:09 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (!commandline || !commandline[0])
		return ;
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

static int	has_pipes(t_mshell_data *data)
{
	t_tokens	*temp;

	temp = data->tokens;
	while (temp)
	{
		if (temp->type == NODE_PIPE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	executor(t_mshell_data *data)
{
	char	**commands;

	expand_all_tokens(data);
	if (!prep_heredoc(data))
		return ;
	if (has_pipes(data))
		return (execute_piped_commands(data, data->tokens), (void)0);
	commands = array_join(data->tokens);
	if (!ft_strcmp(data->tokens->input, ">") || !ft_strcmp(data->tokens->input,
			">>") || !ft_strcmp(data->tokens->input, "<"))
		return (redirect_start(data->tokens, data));
	if (commands && commands[0] && data->tokens->type == NODE_WORD)
	{
		if (!has_redirect(data->tokens))
			run_command(commands, data);
		else if (is_builtin(commands))
			run_builtin_redirects(commands, data);
		else
			ft_execve(commands, data);
	}
}
