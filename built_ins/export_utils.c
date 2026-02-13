/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:01 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 10:03:19 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	identifier_valid(char *str)
{
	int		i;
	char	*start;

	if (!str || !*str)
		return (0);
	i = 0;
	start = (char *)str;
	if (*start == '"' || *start == '\'')
	{
		start++;
		i = 1;
	}
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (start[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Checks env list and sees if there is a match
	return 1 = match
	return 0 = no match
*/

int	check_env_list(char *str, t_mshell_data *data)
{
	t_env	*temp;
	int		len;

	if (!data || !data->env_var || !str)
		return (0);
	len = ft_strlen(str);
	temp = data->env_var;
	while (temp)
	{
		if (!ft_strcmp(str, temp->var)
			|| (!ft_strncmp(str, temp->var, len) && temp->var[len] == '='))
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	add_env_list(char *str, t_mshell_data *data)
{
	t_env	*temp;
	t_env	*new_node;

	if (!data || !data->env_var || !str)
		return (0);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (1);
	new_node->var = str;
	new_node->next = NULL;
	if (!data->env_var)
	{
		data->env_var = new_node;
		return (0);
	}
	temp = data->env_var;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	return (0);
}

int	update_env_list(char *str, t_mshell_data *data)
{
	t_env	*temp;
	int		name_len;

	if (!str || !data || !data->env_var)
		return (1);
	temp = data->env_var;
	name_len = 0;
	while (str[name_len] && str[name_len] != '=')
		name_len++;
	while (temp)
	{
		if (!ft_strncmp(str, temp->var, name_len)
			&& ((temp->var[name_len] == '=' || temp->var[name_len] == '\0')))
		{
			if (ft_strchr(str, '='))
			{
				free(temp->var);
				temp->var = str;
			}
			return (0);
		}
		temp = temp->next;
	}
	return (1);
}
