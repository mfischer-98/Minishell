#include "../minishell.h"

int	identifier_valid(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*remove_quotes(char *str)
{
	//deixamos com quotes quando fizemos os tokens? se nao tirar isso :)
	int	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (((len >= 2) && ((str[0] == '"' && str[len - 1] == '"')))
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

// Checks env list and sees if there is a match
// 	return 1 = match
// 	return 0 = no match

int	check_env_list(char *str, t_mshell_data *data)
{
	t_env	*temp;

	if (!data || !data->env_var || !str)
		return (0);
	temp = data->env_var;
	while (temp)
	{
		if (!ft_strcmp(str, temp->var))
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	add_env_list(char	*str, int value, t_mshell_data *data); //add node to list


int	update_env_list(char	*str, int value, t_mshell_data *data); //update value in lis