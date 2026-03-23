/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:20:55 by mefische          #+#    #+#             */
/*   Updated: 2026/03/23 11:26:42 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Returns length of name before the + in name+=value */
int	get_name_len(char	*str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+')
		i++;
	return (i);
}

/* Extracts the value after += from the full name+=value string
	- Returns NULL if += not found */
char	*get_append_value(char	*str)
{
	char	*value;

	value = ft_strnstr(str, "+=", ft_strlen(str));
	if (!value)
		return (NULL);
	return (ft_strdup(value + 2));
}

/* Finds the var string in env list matching name
	- Returns pointer directly into the list
	- Read old value before building the new appended string */
char	*get_old_var(char *name, int len, t_mshell_data *data)
{
	t_env	*temp;

	temp = data->env_var;
	while (temp)
	{
		if (!ft_strncmp(name, temp->var, len)
			&& (temp->var[len] == '=' || temp->var[len] == '\0'))
			return (temp->var);
		temp = temp->next;
	}
	return (NULL);
}

/* Builds the new var string by appending new_value to old
	- If old_var has = already (NAME=old), just joins old+new
	- If no = (unset var), builds NAME=new from scratch */
char	*build_var(char *name, char *old_var, char *new_value)
{
	char	*temp;
	char	*result;

	if (ft_strchr(old_var, '='))
	{
		temp = ft_strjoin(old_var, new_value);
		if (!temp)
			return (NULL);
	}
	else
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, new_value);
		free(temp);
		return (result);
	}
	return (temp);
}

/* Main handler for += append operator
	- Extracts name and value, finds old var, builds new string,
	updates env list
	- If var doesn't exist yet, adds value */
int	handle_append(char *str, t_mshell_data *data)
{
	int		name_len;
	char	*name;
	char	*value;
	char	*new_str;
	char	*old_var;

	name_len = get_name_len(str);
	name = ft_substr(str, 0, name_len);
	if (!name_len || !name || !identifier_valid(name))
		return (free(name), 1);
	value = get_append_value(str);
	if (!value)
		return (free(name), 1);
	old_var = get_old_var(name, name_len, data);
	if (!old_var)
		return (free(value), free(name), add_env_list(str, data));
	new_str = build_var(name, old_var, value);
	free(value);
	free(name);
	if (!new_str)
		return (1);
	update_env_list(new_str, data);
	free(new_str);
	return (0);
}
