#include "../minishell.h"

char	*ft_get_env_value(t_env *env, char *name)
{
	while (env && ft_strncmp(env->var, name, ft_strlen(name)))
		env = env->next;
	if (env && env->var[ft_strlen(name)] == '=')
		return (env->var + ft_strlen(name) + 1);
	return (NULL);
}

void	ft_set_env_var(t_mshell_data *data, char *name, char *value)
{
	char	*env_str;
	char	*temp;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return ;
	env_str = ft_strjoin(temp, value);
	free(temp);
	if (!env_str)
		return ;
	if (update_env_list(env_str, data))
		add_env_list(env_str, data);
}
