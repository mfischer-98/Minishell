/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:19:33 by mefische          #+#    #+#             */
/*   Updated: 2026/02/17 18:05:43 by mefische         ###   ########.fr       */
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
		free(tokens);
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

int	array_size(t_tokens *tokens)
{
	t_tokens	*temp;
	int			size;

	temp = tokens;
	size = 0;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_WORD && !temp->is_redir_name)
			size++;
		temp = temp->next;
	}
	return (size);
}

char	**array_join(t_tokens *tokens)
{
	char		**array;
	t_tokens	*temp;
	int			i;
	int			size;

	size = array_size(tokens);
	i = 0;
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	temp = tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if ((temp->type == NODE_WORD && !temp->is_redir_name))
		{
			array[i] = ft_strdup(temp->input);
			if (!array[i])
				return (free_array(array, i), NULL);
			i++;
		}
		temp = temp->next;
	}
	array[i] = NULL;
	return (array);
}
