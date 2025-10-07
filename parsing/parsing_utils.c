/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:19:33 by mefische          #+#    #+#             */
/*   Updated: 2025/10/07 12:20:20 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	list_add(t_commands **commands, char *input)
{
	t_commands	*new_node;
	t_commands	*node;

	new_node = malloc(sizeof(t_commands));
	if (!new_node)
	{
		free(commands); //check later
		return ;
	}
	new_node->input = input;
	new_node->next = NULL;
	node = *commands;
	if (*commands == NULL)
	{
		*commands = new_node;
		return ;
	}
	while (node->next != NULL)
		node = node->next;
	node->next = new_node;
}

char	**free_array(char **array, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

void	free_list(t_commands *commands)
{
	t_commands	*temp;

	while (commands != NULL)
	{
		temp = commands;
		commands = commands->next;
		free(temp);
	}
	free(commands);
}
