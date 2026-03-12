/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 00:00:00 by mefische          #+#    #+#             */
/*   Updated: 2026/03/05 00:00:00 by mefische         ###   ########.fr       */
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
    char **envp = list_to_array(env, env_size(env));
    if (!envp)
    {
        perror("malloc envp");
        exit(1);
    }
    return (envp);
}

void	execute_external_command(char **commandline, t_mshell_data *data,
        t_tokens *segment)
{
    char *cmd_path;
    char **envp;

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
    execve(cmd_path, commandline, envp);
    perror("execve");
    free_array(envp, env_size(data->env_var));
    exit(1);
}
