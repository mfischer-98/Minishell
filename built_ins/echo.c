/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntomas-g <ntomas-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:28:00 by mefische          #+#    #+#             */
/*   Updated: 2026/01/23 11:31:40 by ntomas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_flag(char *arg)
{
	int	i;

    i = 1;
	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(char **arg)
{
	int	i;
	int	j;

	if (!arg)
		return (ft_putchar_fd('\n', 1), 0);
	j = 1;
	i = 1;
	while (arg[i] && ft_flag(arg[i]))
	{
		j = 0;
		i++;
	}
	while (arg[i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (j)
		ft_putchar_fd('\n', 1);
	return (0);
}