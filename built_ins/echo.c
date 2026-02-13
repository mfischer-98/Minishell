/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:20:49 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 09:20:49 by mefische         ###   ########.fr       */
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

static void	print_arg(char *str)
{
	int	len;

	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		write(1, str + 1, len - 2);
	else
		ft_putstr_fd(str, 1);
}

int	echo(char **arg)
{
	int	i;
	int	newline;

	if (!arg)
		return (ft_putchar_fd('\n', 1), 0);
	i = 1;
	newline = 1;
	while (arg[i] && ft_flag(arg[i]))
	{
		newline = 0;
		i++;
	}
	while (arg[i])
	{
		print_arg(arg[i]);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
