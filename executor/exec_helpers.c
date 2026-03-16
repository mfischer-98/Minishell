/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 00:00:00 by mefische          #+#    #+#             */
/*   Updated: 2026/03/16 18:49:45 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void print_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	
}

static char **get_envp_or_exit(t_env *env)
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

// TEST
static int  validate_cmd_path(char *cmd_name, char *cmd_path)
{
    struct stat st;

	(void)cmd_name;
    (void)cmd_path;
    // Case 1: Test 1 & 8 - Directory (exists but not executable file)
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
    
    // Case 2: Test 4 - File exists but no execute permission
    if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd_name, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        return (126);
    }
    
    // Case 3: Test 5 & 10 - File doesn't exist (let execve handle)
    return (0);
}

// TEST
void    execute_external_command(char **commandline, t_mshell_data *data,
        t_tokens *segment)
{
    char        *cmd_path;
    char        **envp;
    int         err_code;

    data->exit_status = apply_redirects(segment, data);
    if (data->exit_status != 0)
        exit(data->exit_status);
    
    cmd_path = find_command_in_path(commandline[0], data->env_var);
    if (!cmd_path)
    {
        print_cmd_not_found(commandline[0]);
        exit(127);
    }
    
    // *** USE YOUR HELPER - Clean & 42-norm perfect ***
    err_code = validate_cmd_path(commandline[0], cmd_path);
    if (err_code != 0)
    {
        exit(err_code);
    }
    
    envp = get_envp_or_exit(data->env_var);
    execve(cmd_path, commandline, envp);
    perror("execve");
    free_array(envp, env_size(data->env_var));
    exit(1);
}




/* void	execute_external_command(char **commandline, t_mshell_data *data,
		t_tokens *segment)
{
	char	*cmd_path;
	char	**envp;
	struct stat st;

	data->exit_status = apply_redirects(segment, data);
	if (data->exit_status != 0)
		exit(data->exit_status);
	cmd_path = find_command_in_path(commandline[0], data->env_var);
	if (!cmd_path)
	{
		print_cmd_not_found(commandline[0]);
		data->exit_status = 127;
		exit(127);
	}
	envp = get_envp_or_exit(data->env_var);
	data->exit_status = 1;
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(commandline[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        free_array(envp, env_size(data->env_var));
        exit(126);
    }
	execve(cmd_path, commandline, envp);
	perror("execve");
	free_array(envp, env_size(data->env_var));
	exit(1);
} */
