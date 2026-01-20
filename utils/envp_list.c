#include "../minishell.h"

void create_envp(char **vars, t_mshell_data *data)
{
    int     i;

    i = 0;
    while (vars[i])
    {
        envp_add(data, vars[i]);
        i++;
    }
}

void	envp_add(t_mshell_data *data, char *input)
{
	t_env	*new_node;
	t_env	*node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->var = ft_strdup(input);
	new_node->next = NULL;
	if ((*data).env_var == NULL)
	{
		(*data).env_var = new_node;
		return ;
	}
    node = (*data).env_var;
	while (node->next != NULL)
		node = node->next;
	node->next = new_node;
}

int count_array(char **array) //nao usei
{
    int i;

    i = 0;
    while(array[i])
        i++;
    return (i);
}

void	print_env(t_mshell_data *data)
{
	t_env	*temp;

	temp = data->env_var;
	while (temp)
	{
		ft_printf("%s\n", temp->var);
		temp = temp->next;
	}
}
