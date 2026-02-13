/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:13 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 14:20:42 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_command_in_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	path_env = NULL;
	while (env_list)
	{
		if (ft_strncmp(env_list->var, "PATH=", 5) == 0)
		{
			path_env = env_list->var + 5;
			break ;
		}
		env_list = env_list->next;
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths, i + 1);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths, i);
	return (NULL);
}

static void	execute_external_command(char **commandline, t_mshell_data *data)
{
	char	*cmd_path;
	char	**envp;
	int		size;

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
}

void	run_command(char **commandline, t_mshell_data *data)
{
	pid_t	pid;
	int		status;

	if (!commandline || !commandline[0])
		return ;
	if (!ft_strcmp(commandline[0], "pwd"))
		data->exit_status = pwd();
	/* else if (!ft_strcmp(commandline[0], "cd"))
		data->exit_status = cd(data, commandline); */
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
	else // Not a built-in - fork to then execute
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			data->exit_status = 1;
			return ;
		}
		if (pid == 0) //child
			execute_external_command(commandline, data);
		else //parent
			waitpid(pid, &status, 0);
		data->exit_status = WEXITSTATUS(status);
	}
}

static int	check_unclosed_quotes(t_tokens *tokens)
{
	int			i;
	int			sq;
	int			dq;
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		sq = 0;
		dq = 0;
		i = 0;
		while (temp->input[i])
		{
			if (temp->input[i] == '\"')
				dq++;
			if (temp->input[i] == '\'')
				sq++;
			i++;
		}
		temp = temp->next;
	}
	if ((dq % 2 != 0) || (dq % 2 != 0))
		return (1);
	return (0);
}

void	executor(t_mshell_data *data)
{
	t_tokens	*temp;
	char		*expanded;
	char		**commands;

	if (!data || !data->tokens)
		return ;
	temp = data->tokens;
	if (check_unclosed_quotes(data->tokens))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Error: Unclosed quotes\n", 2);
		data->exit_status = 1;
		return ;
	}
	while (temp)
	{
		expanded = expand_tokens(temp->input, data);
		free(temp->input);
		temp->input = expanded;
		temp = temp->next;
	}
	commands = array_join(data->tokens);
	if (data->tokens && data->tokens->type == NODE_WORD
		&& commands && commands[0])
		run_command(commands, data);
}
