/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:33 by mefische          #+#    #+#             */
/*   Updated: 2026/03/04 16:54:35 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_arg(char **command, t_mshell_data *data)
{
	int	i;

	i = 0;
	if (command[1][i] == '+' || command[1][i] == '-')
	{
		i++;
		if (command[1][i] == '-')
			return (0);
	}
	while (command[1][i] && ft_isdigit(command[1][i]))
		i++;
	
	if (command[1][i] != '\0' || ft_strcmp(command[1], "9223372036854775808") == 0 ||
    (ft_strlen(command[1]) > 19 && command[1][0] != '-') ||
    ft_strlen(command[1]) > 20)
	{
		data->exit_status = 2;
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(command[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}

	return (ft_atoi(command[1]) % 256);
}

int	check_exit(char **command, t_mshell_data *data)
{
	int		exit_code;

	if (!command || !command[0] || ft_strcmp(command[0], "exit") != 0)
		return (1);
	if (command[1] && command[2] && command[2][0] != '\0')
	{
		data->exit_status = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = data->exit_status;
	if (command[1])
		exit_code = exit_arg(command, data);
	ft_printf("exit\n");
	free_data(data);
	rl_clear_history();
	exit(exit_code);
}
