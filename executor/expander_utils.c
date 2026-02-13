/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:29:52 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 14:29:52 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_status(t_mshell_data *data)
{
	char	*temp;
	char	*old;

	temp = ft_itoa(data->exit_status);
	old = data->expander->result;
	data->expander->result = ft_strjoin(data->expander->result, temp);
	free(old);
	free(temp);
}

// Finds environment variable value in the list
char	*get_env_var(char *token, t_mshell_data *data)
{
	t_env	*temp;
	int		len;

	temp = data->env_var;
	len = ft_strlen(token);
	while (temp)
	{
		if (!ft_strncmp(token, temp->var, len)
			&& ((temp->var[len] == '=' || temp->var[len] == '\0')))
			return (ft_strdup(temp->var + len + 1));
		temp = temp->next;
	}
	return (ft_strdup(""));
}
