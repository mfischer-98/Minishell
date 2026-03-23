/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:04 by mefische          #+#    #+#             */
/*   Updated: 2026/03/23 11:19:11 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Skips leading spaces and tabs - used before printing invalid
	identifier errors so the message matches bash output */
static char	*trim_left(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

/* Handles export with no = sign (e.g. export VAR)
	- Validates identifier, then adds to env list without a value */
int	handle_no_equal(char *arg, t_mshell_data *data)
{
	if (arg[0] == '\0')
		return (0);
	if (!identifier_valid(arg))
	{
		ft_putstr_fd("minishell: `", 2);
		ft_putstr_fd(trim_left(arg), 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	return (add_env_list(arg, data));
}

/* Handles export with = sign (e.g. export VAR=value)
	- Extracts name before =, validates it
	- adds (if new) or updates (if exists) the env list */
int	handle_equal(char *arg, t_mshell_data *data)
{
	char	*equal_sign;
	char	*name;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (1);
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1);
	if (!identifier_valid(name))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(trim_left(arg), 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(name);
		return (1);
	}
	if (!check_env_list(name, data))
		add_env_list(arg, data);
	else
		update_env_list(arg, data);
	free(name);
	return (0);
}

/* Export argument goes to the right handler
	- rejects if starts with -
	- strips outer quotes
	- then dispatches to append (+=), equal (=) or no equal handler */
int	process_export(char *str, t_mshell_data *data)
{
	char	*new_str;
	int		ret;

	if (str[0] == '-')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (2);
	}
	new_str = trim_outer_quotes(str);
	if (ft_strnstr(new_str, "+=", ft_strlen(new_str)))
		ret = handle_append(new_str, data);
	else if (ft_strchr(new_str, '='))
		ret = handle_equal(new_str, data);
	else
		ret = handle_no_equal(new_str, data);
	free(new_str);
	return (ret);
}

/* Entry point for the export builtin:
	- no args = prints all exported variables
	- with args = processes each one
	- keeps worst error as final status */
int	export(char **commandline, t_mshell_data *data)
{
	int	status;
	int	final_status;
	int	i;

	if (!data || !commandline || !commandline[0])
		return (1);
	if (!commandline[1])
	{
		print_export(data);
		return (0);
	}
	status = 0;
	final_status = 0;
	i = 1;
	while (commandline[i])
	{
		status = process_export(commandline[i], data);
		if (status != 0)
			final_status = status;
		i++;
	}
	return (final_status);
}
