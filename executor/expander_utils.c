/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:29:52 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 14:29:52 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Checks if we have closed brackets */
static int	valid_bracket(char	*str)
{
	int	i;
	int	open;
	int	close;

	open = 0;
	close = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '{')
			open++;
		else if (str[i] == '}')
			close++;
		i++;
	}
	if (open != close && open >= 1)
		return (0);
	return (1);
}

/* Finds brackets and expands what is inside followed by text */
char	*handle_brackets(char	*str, int len, int start, t_mshell_data *data)
{
	char	*var_name;
	int		i;

	if (!valid_bracket(str))
		return (NULL);
	i = start + 1;
	while (i < len && str[i] && str[i] != '}')
		i++;
	if (i >= len || str[i] != '}')
		return (NULL);
	len = i - (start + 1);
	var_name = ft_substr(str, start + 1, len);
	if (!var_name)
		return (NULL);
	data->expander->i = i + 1;
	return (var_name);
}

/* Error handler for unclosed ${VAR
	- prints error, skips rest of input
	- exit_status to 127 */
void	var_name_error(char *input, t_mshell_data *data)
{
	ft_printf("minishell: unclosed the brackets\n");
	data->expander->i = ft_strlen(input);
	data->exit_status = 127;
}

/* Appends a string to expander->result with ft_strjoin
	- Handles NULL input safely by substituting empty string
	- Always frees old result after joining */
void	append_result(char	*str, t_mshell_data *data)
{
	char	*temp;
	char	*old;

	if (str == NULL)
		temp = ft_strdup("");
	else
		temp = ft_strdup(str);
	old = data->expander->result;
	data->expander->result = ft_strjoin(data->expander->result, temp);
	free(old);
	free(temp);
}

/* Handles $? and $_ special variables
	- $? → converts exit_status int to string and appends
	- $_ → finds "_=" in env list and appends its value */
void	special_char(char c, t_mshell_data *data)
{
	t_env	*temp;
	char	*value;

	if (c == '?')
	{
		value = ft_itoa(data->exit_status);
		append_result(value, data);
		free(value);
	}
	else if (c == '_')
	{
		value = NULL;
		temp = data->env_var;
		while (temp)
		{
			if (!ft_strncmp(temp->var, "_=", 2))
			{
				value = temp->var + 2;
				break ;
			}
			temp = temp->next;
		}
		append_result(value, data);
	}
	data->expander->i++;
}
