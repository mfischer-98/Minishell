/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:16 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 14:33:52 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Expander
** If $VAR:
		1- If VAR exists -> replace by value
		2- If VAR does not exist new line
		3- id '$VAR' -> do not expand
** If $?: replace by exit status number

** Get_var_len
** Calculates valid variable name length after $
** Valid: [a-zA-Z_][a-zA-Z0-9_]*
** Returns: length or 0 if invalid
*/
static int	get_var_len(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (!ft_isalpha((unsigned char)str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && (ft_isalnum((unsigned char)str[i]) || str[i] == '_'))
		i++;
	return (i);
}

/*
** Toggles single/double quote states
** Returns 1 if quote handled (skip char), 0 otherwise
** Updates expander->i on toggle
*/
static int	handle_quotes(char c, t_expander *expander)
{
	if (c == '\'' && !expander->in_double)
	{
		expander->in_single = !expander->in_single;
		expander->i++;
		return (1);
	}
	if (c == '\"' && !expander->in_single)
	{
		expander->in_double = !(expander->in_double);
		expander->i++;
		return (1);
	}
	return (0);
}

/*
** Handles $VAR or $? expansion
** Updates expander->result and expander->i
*/
static void	handle_var(char *input, t_mshell_data *data)
{
	int		var_len;
	char	*var_name;
	char	*temp;

	data->expander->i++;
	if (input[data->expander->i] == '?')
	{
		append_status(data);
		data->expander->i++;
		return ;
	}
	var_len = get_var_len(input + data->expander->i);
	if (var_len == 0)
	{
		temp = ft_strdup("$");
		data->expander->result = ft_strjoin(data->expander->result, temp);
		free(temp);
		return ;
	}
	var_name = ft_substr(input + data->expander->i, 0, var_len);
	temp = get_env_var(var_name, data);
	data->expander->result = ft_strjoin(data->expander->result, temp);
	free(var_name);
	free(temp);
	data->expander->i += var_len;
}

char	*expand_tokens(char *input, t_mshell_data *data)
{
	char		*temp;
	t_expander	*exp;

	init_expander(&data->expander);
	exp = data->expander;
	while (input[exp->i])
	{
		if (handle_quotes(input[exp->i], exp))
			continue ;
		if (input[exp->i] == '$' && !exp->in_single)
		{
			handle_var(input, data);
			continue ;
		}
		temp = ft_substr(&input[exp->i], 0, 1);
		exp->result = ft_strjoin(exp->result, temp);
		free(temp);
		exp->i++;
	}
	return (exp->result);
}
