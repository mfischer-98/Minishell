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
		if (temp->type == NODE_IN || temp->type == NODE_OUT ||
			temp->type == NODE_APPEND || temp->type == NODE_HERE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

static void	builtin_call(char **commandline, t_mshell_data *data)
{
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
		builtin_call(commandline, data);
	dup2(old_stdin, 0);
	dup2(old_stdout, 1);
	close(old_stdin);
	close(old_stdout);
}

/* static void	execute_external_command(char **commandline, t_mshell_data *data)
{
	char	*cmd_path;
	char	**envp;
	int		size;

	if (apply_redirects(data->tokens))
		exit(1);
	cmd_path = find_command_in_path(commandline[0], data->env_var);
	if (!cmd_path)
	{
		ft_printf("minishell: command not found: %s\n", commandline[0]);
		exit(127);
	}
	size = env_size(data->env_var);
	envp = list_to_array(data->env_var, size);
	if (!envp)
	{
		perror("malloc envp");
		exit(1);
	}
	execve(cmd_path, commandline, envp);
	perror("execve");
	free_array(envp, size);
	exit(1);
} */

/* void	executor(t_mshell_data *data)
{
	char	**commands;

	if (!data || !data->tokens)
		return ;
	if (check_unclosed_quotes(data->tokens))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Error: Unclosed quotes\n", 2);
		return (data->exit_status = 1, (void)0);
	}
	expand_all_tokens(data);
	if (!prep_heredoc(data))
		return ;
	if (has_pipes(data))
		execute_piped_commands(data, data->tokens);
	else
	{
		commands = array_join(data->tokens);
		if (data->tokens && data->tokens->type == NODE_WORD
			&& commands && commands[0])
		{
			if (!has_redirect(data->tokens))
				run_command(commands, data);
			else
				run_builtin_redirects(commands, data);
		}
	}
	//free_array(commands, array_size(commands));
} */