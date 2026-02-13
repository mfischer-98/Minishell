/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:33 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 13:02:09 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_exit(char **command, t_mshell_data *data)
{
	int		exit_code;
	int		i;

	if (!command || !command[0] || ft_strcmp(command[0], "exit") != 0)
		return (1);
	if (command[2])
	{
		data->exit_status = 1;
		return (ft_printf("exit: too many arguments\n"), 1);
	}
	exit_code = data->exit_status;
	if (command[1])
	{
		i = 0;
		while (command[1][i] && ft_isdigit(command[1][i]))
			i++;
		if (command[1][i] != '\0')
		{	
			data->exit_status = 2;
			return (ft_printf("exit: %s: invalid argument\n", command[1]), 1);
		}
		exit_code = ft_atoi(command[1]) % 256;
	}
	ft_printf("exit\n");
	exit(exit_code);
}
