#include "../minishell.h"

int	get_name_len(char	*str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+')
		i++;
	return (i);
}

char *get_append_value(char	*str)
{
	char	*value;

	value = ft_strnstr(str, "+=", ft_strlen(str));
	if (!value)
		return (NULL);
	return (value + 2);
}

char *get_old_var(char *name, int len, t_mshell_data *data)
{
	t_env	*temp;

	temp = data->env_var;
	while (temp)
	{
		if (!ft_strncmp(name, temp->var, len) &&
			(temp->var[len] == '=' || temp->var[len] == '\0'))
			return (temp->var);
		temp = temp->next;
	}
	return (NULL);
}

char	*build_var(char *name, char *old_var, char *new_value)
{
	char	*temp;

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
		temp = ft_strjoin(temp, new_value);
		if (!temp)
			return (NULL);
	}
	return (temp);
}

int	handle_append(char *str, t_mshell_data *data) //str: name+=value
{
	int		name_len;
	char	*name;
	char	*value;
	char	*new_str;
	char	*old_var;

	name_len = get_name_len(str);
	if (!name_len || !(name = ft_substr(str, 0, name_len)) || !identifier_valid(name))
		return (1);
	if (!(value = get_append_value(str)))
		return (free(name), 1);
	old_var = get_old_var(name, name_len, data);
	if (!old_var)
		return (free(name), add_env_list(str, data));
	new_str = build_var(name, old_var, value);
	free(name);
	if (!new_str)
		return (1);
	update_env_list(new_str, data);
	return (0);
}
