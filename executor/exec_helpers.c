/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 00:00:00 by mefische          #+#    #+#             */
/*   Updated: 2026/03/23 12:08:57 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_perm_denied(char **cmd, t_mshell_data *data)
{
	struct stat	st;

	ft_memset(&st, 0, sizeof(st));
	if (access(cmd[0], F_OK) != 0)
	{
		if (!S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			error_free(data, cmd);
			exit(127);
		}
		print_cmd_not_found(cmd[0]);
		exit(127);
	}
	if (access(cmd[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		error_free(data, cmd);
		exit(126);
	}
}

static char	**get_envp_or_exit(t_env *env)
{
	char	**envp;

	envp = list_to_array(env, env_size(env));
	if (!envp)
	{
		perror("malloc envp");
		exit(1);
	}
	return (envp);
}

/* Validates that cmd_path is not a directory and is executable
	- Uses stat() to check file metadata, S_ISDIR to detect directories
	- tat() asks the OS: "tell me everything about this file" and 
	fills a struct stat with metadata
		- it doesn't open the file, just reads its info
	- Returns 126 for "is a directory" or "permission denied", 0 if valid */
static int	validate_cmd_path(char *cmd_name, char *cmd_path)
{
	struct stat	st;

	(void)cmd_name;
	(void)cmd_path;
	ft_memset(&st, 0, sizeof(st));
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_name, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			return (126);
		}
	}
	if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (0);
}

void	execute_external_command(char **commandline, t_mshell_data *data,
		t_tokens *segment)
{
	char	*cmd_path;
	char	**envp;
	int		err_code;

	if (ft_strchr(commandline[0], '/'))
		print_perm_denied(commandline, data);
	err_code = apply_redirects(segment, data);
	if (err_code != 0)
		return (free_array(commandline, array_size(commandline)),
			free_data(data), exit(err_code), (void)0);
	cmd_path = find_command_in_path(commandline[0], data->env_var);
	if (!cmd_path)
	{
		print_cmd_not_found(commandline[0]);
		return (free_array(commandline, array_size(commandline)),
			free_data(data), exit(127), (void)0);
	}
	err_code = validate_cmd_path(commandline[0], cmd_path);
	if (err_code != 0)
		return (free(cmd_path), free_array(commandline, array_size
				(commandline)), free_data(data), exit(err_code), (void)0);
	envp = get_envp_or_exit(data->env_var);
	execve(cmd_path, commandline, envp);
	return (perror("execve"), free_array(commandline, array_size(commandline)),
		free_data(data), exit(1), (void)0);
}
