/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:48:36 by mefische          #+#    #+#             */
/*   Updated: 2026/03/04 16:53:58 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_command_in_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	while (env_list && ft_strncmp(env_list->var, "PATH=", 5) != 0)
		env_list = env_list->next;
	paths = (env_list ? ft_split(env_list->var + 5, ':') : NULL);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = (tmp ? ft_strjoin(tmp, cmd) : NULL);
		if (full_path && access(full_path, X_OK) == 0)
			return (free(tmp), free_array(paths, i + 1), full_path);
		free(tmp);
		free(full_path);
	}
	return (free_array(paths, i), NULL);
}

char	**build_command(t_tokens **tokens)
{
	int			count;
	int			i;
	t_tokens	*temp;
	char		**cmd;

	count = 0;
	temp = *tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		count += (temp->type == NODE_WORD);
		temp = temp->next;
	}
	cmd = malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	temp = *tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_WORD)
			cmd[i++] = temp->input;
		temp = temp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

void	expand_all_tokens(t_mshell_data *data)
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
}

void	execute_external_command(char **commandline, t_mshell_data *data,
		t_tokens *segment)
{
	char	*cmd_path;
	char	**envp;

	data->exit_status = apply_redirects(segment, data);
	if (data->exit_status != 0)
		exit(data->exit_status);
	cmd_path = find_command_in_path(commandline[0], data->env_var);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(commandline[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		data->exit_status = 127;
		exit(127);
	}
	envp = list_to_array(data->env_var, env_size(data->env_var));
	data->exit_status = 1;
	if (!envp)
	{
		perror("malloc envp");
		exit(1);
	}
	execve(cmd_path, commandline, envp);
	perror("execve");
	free_array(envp, env_size(data->env_var));
	exit(1);
}

void	execute_piped_commands(t_mshell_data *data, t_tokens *tokens)
{
	pid_t		pid;
	int			pipefd[2];
	char		**cmd;
	t_tokens	*next;
	int			saved;

	next = tokens;
	while (next && next->type != NODE_PIPE)
		next = next->next;
	cmd = build_command(&tokens);
	if (!cmd || (saved = dup(STDIN_FILENO)) == -1)
		return ;
	if (next)
		pipe(pipefd);
	if ((pid = fork()) == 0)
	{
		if (next)
			dup2(pipefd[1], STDOUT_FILENO);
		execute_external_command(cmd, data, tokens);
	}
	if (next)
		dup2(pipefd[0], STDIN_FILENO), execute_piped_commands(data, next->next);
	waitpid(pid, 0, 0);
	dup2(saved, STDIN_FILENO);
}
