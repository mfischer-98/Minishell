/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:31:26 by mefische          #+#    #+#             */
/*   Updated: 2026/03/05 09:35:56 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (perror("pwd"), 1);
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(path);
	return (0);
}

static void	env_underscore(t_mshell_data	*data)
{
	t_env	*temp;

	temp = data->env_var;
	while (temp)
	{
		if (!ft_strncmp(temp->var, "_=", 2))
		{
			free(temp->var);
			temp->var = ft_strdup("_=/usr/bin/env");
			break ;
		}
		temp = temp->next;
	}
}

int	env(char **commandline, t_mshell_data *data)
{
	if (!data->env_var)
	{
		perror("Error in environment variables.");
		return (0);
	}
	if (commandline[1])
	{
		ft_printf("env: '%s': No such file or directory\n", commandline[1]);
		return (1);
	}
	env_underscore(data);
	print_env(data);
	return (0);
}
