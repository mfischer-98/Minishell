/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:19:33 by mefische          #+#    #+#             */
/*   Updated: 2025/10/22 16:31:36 by mefische         ###   ########.fr       */
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

char	**array_join(t_tokens **tokens)
{
	char		**array;
	t_tokens	*temp;
	int			i;
	int			len;
	
	len = 0;
	i = 0;
	temp = *tokens;
	while (temp)
	{
    	if (temp->type == NODE_WORD)
        	len++;
    	temp = temp->next;
	}
	array = NULL;
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return NULL;
	temp = *tokens;
	while(temp)
	{
		if(temp->type == NODE_WORD)
		{
			array[i] = temp->input;
			i++;
		}
		temp = temp->next;
	}
	array[i] = NULL;
	return(array);
}
