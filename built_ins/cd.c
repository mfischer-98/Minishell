/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:12:02 by mefische          #+#    #+#             */
/*   Updated: 2026/02/02 14:41:13 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_get_env_value(t_env *env, char *name)
{
	while (env && ft_strncmp(env->var, name, ft_strlen(name)))
		env = env->next;
	if (env && env->var[ft_strlen(name)] == '=')
		return (env->var + ft_strlen(name) + 1);
	return (NULL);
}

static char	*ft_destination(t_mshell_data *data, char *arg)
{
	char	*value;

	if (!arg || ft_strcmp(arg, "~") == 0 || ft_strcmp(arg, "--") == 0)
	{
		value = ft_get_env_value(data->env_var, "HOME");
		if (!value || value[0] == '\0')
			return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
		return (value);
	}
	if (ft_strcmp(arg, "-") == 0)
	{
		value = ft_get_env_value(data->env_var, "OLDPWD");
		if (!value)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), NULL);
		ft_putendl_fd(value, 1);
		return (value);
	}
	return (arg);
}

static int	ft_cd_env(t_mshell_data *data, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
		ft_set_env_var(&data->env_var, "OLDPWD", old_pwd);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("minishell: cd"), 1);
	ft_set_env_var(&data->env_var, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

static int	ft_try_cdpath(t_mshell_data *data, char *dest, char *old_pwd)
{
	char	**paths;
	char	full[PATH_MAX];
	int		i;

	paths = ft_split(ft_get_env_value(data->env_var, "CDPATH"), ':');
	if (!paths)
		return (-1);
	i = -1;
	while (paths[++i])
	{
		ft_strlcpy(full, paths[i], PATH_MAX);
		ft_strlcat(full, "/", PATH_MAX);
		ft_strlcat(full, dest, PATH_MAX);
		if (!chdir(full))
			return (ft_putendl_fd(full, 1), free_array(paths, i + 1), ft_cd_env(data, old_pwd));
	}
	free_array(paths, i);
	return (-1);
}

int	cd(t_mshell_data *data, char **args)
{
	char	*dest;
	char	*old_pwd;
	int		result;

	if (args[1] && args[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	dest = ft_destination(data, args[1]);
	if (!dest || !(old_pwd = getcwd(NULL, 0)))
		return (1);
	if (!chdir(dest))
		return (ft_cd_env(data, old_pwd));
	if (dest[0] != '/' && (result = ft_try_cdpath(data, dest, old_pwd)) >= 0)
		return (free(old_pwd), result);
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(dest, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return (free(old_pwd), 1);
}
