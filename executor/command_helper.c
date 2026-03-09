/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:48:36 by mefische          #+#    #+#             */
/*   Updated: 2026/03/09 10:45:33 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *search_paths(char **paths, char *cmd)
{
    char *tmp;
    char *full_path;
    int i = 0;

    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = tmp ? ft_strjoin(tmp, cmd) : NULL;
        if (full_path && access(full_path, X_OK) == 0)
            return (free(tmp), free_array(paths, i + 1), full_path);
        free(tmp);
        free(full_path);
        i++;
    }
    free_array(paths, i);
    return (NULL);
}

char    *find_command_in_path(char *cmd, t_env *env_list)
{
    char **paths;

    if (access(cmd, X_OK) == 0)
        return (cmd);
    while (env_list && ft_strncmp(env_list->var, "PATH=", 5) != 0)
        env_list = env_list->next;
    paths = env_list ? ft_split(env_list->var + 5, ':') : NULL;
    if (!paths)
        return (NULL);
    return (search_paths(paths, cmd));
}

char	**build_command(t_tokens **tokens)
{
	int			count;
	int			i;
	t_tokens	*temp;
	char		**cmd;

	count = 0;
	temp = *tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		count += (temp->type == NODE_WORD);
		temp = temp->next;
	}
	cmd = malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	temp = *tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_WORD)
			cmd[i++] = temp->input;
		temp = temp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

void	expand_all_tokens(t_mshell_data *data)
{
	t_tokens	*temp;
	char		*expanded;

	temp = data->tokens;
	while (temp)
	{
		expanded = expand_tokens(temp->input, data);
		free(temp->input);
		temp->input = expanded;
		temp = temp->next;
	}
}
