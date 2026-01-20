#include "../minishell.h"

void	initialize(t_mshell_data **data, char **envp)
{
	*data = malloc(sizeof(t_mshell_data));
	if (!data)
	{
		perror("malloc");
		return ;
	}
	(*data)->tokens = NULL;
	(*data)->local_var = NULL;
	(*data)->env_var = NULL;
	create_envp(envp, *data);
}

void	check_vars(char *token, t_local_var **local_vars)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while(token[i])
	{
		if (token[i] == ' ')
		{
			flag = 0;
			break;
		}
		if (token[i] == '=' && isalnum(token[i+1]))
			flag = 1;
		i++;
	}
	if (flag == 1)
		add_var(token, local_vars);
}

void	add_var(char *token, t_local_var **local_vars)
{
	t_local_var	*new_node;
	t_local_var	*node;

	new_node = malloc(sizeof(t_local_var));
	if (!new_node)
	{
		free(local_vars); //check later
		return ;
	}
	new_node->var = token;
	new_node->next = NULL;
	node = *local_vars;
	if (*local_vars == NULL)
	{
		*local_vars = new_node;
		return ;
	}
	while (node->next != NULL)
		node = node->next;
	node->next = new_node;
}

void	print_vars(t_local_var *vars)
{
	t_local_var	*temp;

	temp = vars;
	while (temp)
	{
		printf("Variavel: %s\n", temp->var);
		temp = temp->next;
	}
}