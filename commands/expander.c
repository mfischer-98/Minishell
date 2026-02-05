#include "../minishell.h"

//Expander
//If $VAR:
//		1- If VAR exists -> replace by value
//		2- If VAR does not exist new line
//		3- id '$VAR' -> do not expand
//If $?: replace by exit status number

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

static int	handle_quotes(char c, int *in_single, int *in_double, int *i)
{
	if (c == '\'' && !(*in_double))
	{
		*in_single = !*in_single;
		(*i)++;
		return (1);
	}
	if (c == '\"' && !(*in_single))
	{
		*in_double = !(*in_double);
		(*i)++;
		return (1);
	}
	return (0);
}

// Gets var-name from list
char	*get_env_var(char *token, t_mshell_data *data) 
{
	t_env	*temp;
	int		len;

	temp = data->env_var;
	len = ft_strlen(token);
	while(temp)
	{
		if (!ft_strncmp(token, temp->var, len) 
			&& ((temp->var[len] == '=' || temp->var[len] == '\0')))
			return (ft_strdup(temp->var + len + 1));
		temp = temp->next;
	}
	return (ft_strdup(""));
}

static void	handle_var(char *input, int *i, t_mshell_data *data, char **res)
{
	int		var_len;
	char	*var_name;
	char	*temp;

	(*i)++;
	if (input[*i] == '?')
	{
		*res = ft_strjoin(*res, ft_itoa(data->exit_status));
		(*i)++;
		return ;
	}
	var_len = get_var_len(input + *i);
	if (var_len == 0)
	{
		temp = ft_strdup("$");
		*res = ft_strjoin(*res, temp);
		free(temp);
		return ;
	}
	var_name = ft_substr(input + *i, 0, var_len);
	temp = get_env_var(var_name, data);
	*res = ft_strjoin(*res, temp);
	free(var_name);
	free(temp);
	*i += var_len;
}


char	*expand_tokens(char *input, t_mshell_data *data)
{
	char	*result;
	char	*temp;
	int		i;
	int		in_single;
	int		in_double;

	result = ft_strdup("");
	i = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		if (handle_quotes(input[i], &in_single, &in_double, &i))
			continue ;
		if (input[i] == '$' && !in_single) 
		{
			handle_var(input, &i, data, &result);
			continue;
		}
		temp = ft_substr(&input[i], 0, 1);
		result = ft_strjoin(result, temp);
		free(temp);
		i++;
	}
	return (result);
}
