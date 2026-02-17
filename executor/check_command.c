/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:13 by mefische          #+#    #+#             */
/*   Updated: 2026/02/16 15:02:00 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_command_in_path(char *cmd, t_env *env_list)
static char	**get_paths_array(char *path_env)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;
	char	*temp;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	path_env = NULL;

	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

static char	*get_path_env(t_env *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->var, "PATH=", 5) == 0)
			return (env_list->var + 5);
		env_list = env_list->next;
	}
	return (NULL);
}

static char	*join_path_cmd(char *path, char *cmd)
{
	char *tmp;
	char *full;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static char	*find_in_paths(char **paths, char *cmd)
{
	char *full_path;
	int i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		{
			temp = ft_strjoin(full_path, cmd);
			free(full_path);
			full_path = temp;
		}
		full_path = join_path_cmd(paths[i], cmd);
		if (!full_path)
		{
			i++;
			continue;
		}
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

static char *find_command_in_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_env;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	path_env = get_path_env(env_list);
	paths = get_paths_array(path_env);
	if (!paths)
		return (NULL);
	return (find_in_paths(paths, cmd));
}
static int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

static void	fill_envp(char **envp, t_env *env_list, int size)
{
	int	i;

	i = 0;
	while (i < size && env_list && env_list->var)
	{
		envp[i] = env_list->var;
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
}

static void execute_external_command(char **commandline, t_env *env_list)
{
	char	*cmd_path;
	char	**envp;
	int		size;
	int		size;

	cmd_path = find_command_in_path(commandline[0], data->env_var);
	if (!cmd_path)
	{
		ft_printf("minishell: command not found: %s\n", commandline[0]);
		exit(127);
	}
	size = env_size(data->env_var);
	envp = list_to_array(data->env_var, size);
	size = count_env_vars(env_list);
	envp = malloc(sizeof(char *) * (size + 2));
	if (!envp)
	{
		perror("malloc envp");
		exit(1);
	}
		return ;
	fill_envp(envp, env_list, size);
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
		pwd();
	else if (!ft_strcmp(commandline[0], "cd"))
	 	cd(data, commandline);
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
	 	unset(commandline, data);
	else // not builtin
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			data->exit_status = 1;
			return ;
		}
		if (pid == 0) //child, reset signals to default
		{
			sig_default(SIGINT); //dies with ctrl+C
			sig_default(SIGQUIT); //dumps if needed
			execute_external_command(commandline, data);
		}
		if (pid == 0) //child
			execute_external_command(commandline, data->env_var);
		else //parent
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)) //if parent caught signal first overrides g_signal
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = 128 + WTERMSIG(status);
		}
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
	if ((dq % 2 != 0) || (sq % 2 != 0))
		return (1);
	return (0);
}

void	executor(t_mshell_data *data)
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

static void	setup_child_pipe(int *pipefd, int has_next)
{
	if (has_next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
	}
}

static void	setup_parent_pipe(int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
}

static void	execute_piped_commands(t_mshell_data *data, t_tokens *tokens)
{
	pid_t		pid;
	int			pipefd[2];
	char		**cmd;
	t_tokens	*next_cmd;
	int			saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	next_cmd = tokens;
	while (next_cmd && next_cmd->type != NODE_PIPE)
		next_cmd = next_cmd->next;
	cmd = build_command(&tokens);
	if (!cmd)
		return ;
	if (next_cmd)
		pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		setup_child_pipe(pipefd, (next_cmd != NULL));
		execute_external_command(cmd, data->env_var);
	}
	if (next_cmd)
	{
		setup_parent_pipe(pipefd);
		execute_piped_commands(data, next_cmd->next);
	}
	waitpid(pid, 0, 0);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

static void	expand_all_tokens(t_mshell_data *data)
{
	t_tokens	*temp;
	char		*expanded;

	temp = data->tokens;
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
	free_array(commands, array_size(data->tokens));
}
