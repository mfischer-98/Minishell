#include "../minishell.h"

t_env	*ft_env_var(t_env *env_list, char *var_name)
{
	t_env	*current;

	if (!env_list || !var_name)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->var, var_name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}