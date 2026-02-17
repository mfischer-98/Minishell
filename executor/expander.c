/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:16 by mefische          #+#    #+#             */
/*   Updated: 2026/02/16 11:55:15 by mefische         ###   ########.fr       */
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

// Finds environment variable value in the list
char	*get_env_var(char *token, t_mshell_data *data)
{
	t_env	*temp;
	int		len;

	temp = data->env_var;
	len = ft_strlen(token);
	while (temp)
	{
		if (!ft_strncmp(token, temp->var, len)
			&& ((temp->var[len] == '=' || temp->var[len] == '\0')))
			return (ft_strdup(temp->var + len + 1));
		temp = temp->next;
	}
	return (ft_strdup(""));
}

/*
** Handles $VAR or $? expansion
** Updates expander->result and expander->i
*/
static void	handle_var(char *input, t_mshell_data *data)
{
	int		var_len;
	char	*var_name;
	char	*value;

	data->expander->i++;
	if (input[data->expander->i] == '?')
		return (append_result(ft_itoa(data->exit_status), data),
			data->expander->i++, (void)0);
	if (input[data->expander->i] == '{')
	{
		var_name = handle_brackets(input, ft_strlen(input),
				data->expander->i, data);
		if (!var_name)
			return (var_name_error(input, data));
	}
	else
	{
		var_len = get_var_len(input + data->expander->i);
		if (var_len == 0)
			return (append_result("$", data));
		var_name = ft_substr(input + data->expander->i, 0, var_len);
		data->expander->i += var_len;
	}
	value = get_env_var(var_name, data);
	return (append_result(value, data), free(value), free(var_name));
}

char	*expand_tokens(char *input, t_mshell_data *data)
{
	char		*temp;
	t_expander	*exp;
	char		*old;

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
		old = exp->result;
		exp->result = ft_strjoin(old, temp);
		free(old);
		free(temp);
		exp->i++;
	}
	temp = ft_strdup(exp->result);
	return (free(exp->result), exp->result = NULL, temp);
}
