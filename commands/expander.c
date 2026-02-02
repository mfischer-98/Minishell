#include "../minishell.h"

//Expander
//If $VAR:
//		1- If VAR exists -> replace by value
//		2- If VAR does not exist nothing 
//If $?: replace by exit status number

//Need to add more functiosn to work with hello$USER $PATH$USER $USER$

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

char	*expand_tokens(char *input, t_mshell_data *data)
{
	if (input[0] != '$')
		return (ft_strdup(input));
	// if (!ft_strcmp(input, "$?"))
	// 	return (ft_itoa(g_exit_status));
	if (input[1] == '\0')
		return (ft_strdup("$"));
	//starts with $ and has a name next to it
	return (get_env_var(input + 1, data));
}
