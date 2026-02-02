#include "../minishell.h"

char	*trim_quotes(char *value)
{
	int		len;

	len = ft_strlen(value);
	if ((value[0] == '"' && value[len - 1] == '"') ||
		(value[0] == '\'' && value[len - 1] == '\''))
		return (ft_substr(value, 1, len - 2));
	return (ft_strdup(value));
}

char	*get_value(char *str, int len)
{
	int		i;
	int		value_len;
	char	*value;

	i = len;
	while (str[i])
	i++;
	value_len = len + i;
	value = malloc(sizeof (char) * (value_len + 1);
	if (!value)
		return (NULL);
	i = 0;
	while (str[len])
	{
		value[i] = str[len]
		i++;
		len++;
	}
	value[i] = '\0';
	return (value);
}

void	add_value();
void	

int	handle_append(char *str, t_mshell_data *data) //str: name+=value
{
	int		name_len;
	char	*value;
	t_env	*temp;
	int		flag;

	if (!str || !data || !data->env_var)
		return (1);'
	while (str[name_len] && str[name_len] != '=')
		name_len++;
	value = get_value(str, name_len);
	temp = t_mshell_data->env_var;
	flag = 0;
	while (temp)
	{
		//name match: if = do it one way if no, do it another way
		if (!ft_strncmp(str, temp->var, name_len) && ((temp->var[name_len] == '=' || temp->var[name_len] == '\0')))
		{
			if (ft_strchr(str, '='))
				append_value();
			else 
				add_value();
			flag = 1;
		}
		temp = temp->next;
	}
	if (flag == 0)
		add_env_list(str);'
	free(value);
	return (0);
}