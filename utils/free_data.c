/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:36 by mefische          #+#    #+#             */
/*   Updated: 2026/02/17 18:19:48 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_data(t_mshell_data *data)
{
	free_env(data->env_var);
	if (data->tokens)
		free_list(data->tokens);
	if (data->expander)
	{
		if (data->expander->result)
			free(data->expander->result);
		free(data->expander);
	}
	free(data);
	rl_clear_history();
}

void	free_array(char **array, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_list(t_tokens *tokens)
{
	t_tokens	*temp;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->input)
			free(temp->input);
		if (temp->redir_file)
			free(temp->redir_file);
		free(temp);
	}
}

void	free_env(t_env *env)
{
	t_env	*temp;

	while (env != NULL)
	{
		temp = env;
		env = env->next;
		if (temp->var)
			free(temp->var);
		free(temp);
	}
}
