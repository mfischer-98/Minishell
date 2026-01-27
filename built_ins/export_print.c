#include "../minishell.h"

char	**list_to_array(t_env *env_list, int size)
{
	char	**env_array; //ou t_env
	t_env	*temp;
	int		i;

	i = 0;
	temp = env_list;
	env_array = malloc (sizeof (char *) * (size + 1));
	if (!env_array)
		return (NULL);
	while (temp && i < size)
	{
		env_array[i] = ft_strdup(temp->var);
		temp = temp->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	bubble_sort_export(char **env_array, int size)
{
	int	i;
	int	j;
	char	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	has_quotes(char	*str)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			double_quotes++;
		else if (str[i] == '\'')
			single_quotes++;
		i++;
	}
	return ((double_quotes == 2 && !single_quotes)
		|| (single_quotes == 2 && !double_quotes));
}

void	print_variable(char *str)
{
	char	*equal_sign;
	int		name_len;
	char	*name;
	char	*value;
	char	*new_value;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
	{
		ft_printf("declare -x %s\n", str);
		return ;
	}
	name_len = equal_sign - str;
	name = ft_substr(str, 0, name_len);
	value = equal_sign + 1;
	if (has_quotes(str))
	{
		new_value = ft_strtrim(value, "\"\'");
		ft_printf("declare -x %s=\"%s\"\n", name, new_value);
		free(new_value);
	}
	else
		ft_printf("declare -x %s=\"%s\"\n", name, value);
	free(name);
}

int print_export(t_mshell_data *data)
{
	char	**env_array;
	int		size;
	int		i;

	size = env_size(data->env_var);
	if (size == 0)
		return (0);
	env_array = list_to_array(data->env_var, size);
	if (!env_array)
		return (1);
	bubble_sort_export(env_array, size);
	i = 0;
	while (i < size)
	{
		if (ft_strncmp(env_array[i], "_=", 2) != 0)
			print_variable(env_array[i]);
		i++;
	}
	free_array(env_array, size);
	return (0);
}
