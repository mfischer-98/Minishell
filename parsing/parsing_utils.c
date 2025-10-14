/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:19:33 by mefische          #+#    #+#             */
/*   Updated: 2025/10/14 12:06:11 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	list_add(t_tokens **tokens, char *input)
{
	t_tokens	*new_node;
	t_tokens	*node;

	new_node = malloc(sizeof(t_tokens));
	if (!new_node)
	{
		free(tokens); //check later
		return ;
	}
	new_node->input = input;
	new_node->next = NULL;
	node = *tokens;
	if (*tokens == NULL)
	{
		*tokens = new_node;
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

void	print_list(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		printf("Input: %s\n", temp->input);
		printf("Type: %d\n", temp->type);
		temp = temp->next;
	}
}

void	free_list(t_tokens *tokens)
{
	t_tokens	*temp;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp);
	}
	free(tokens);
}
