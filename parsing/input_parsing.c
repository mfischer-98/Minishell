/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:12 by mefische          #+#    #+#             */
/*   Updated: 2025/10/14 12:45:58 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_type(t_tokens **tokens)
{
	t_tokens	*temp;

	temp = *tokens;
	while (temp != NULL)
	{
		if (!ft_strcmp(temp->input, "|"))
			temp->type = NODE_PIPE;
		else if (!ft_strcmp(temp->input, ">>"))
			temp->type = NODE_APPEND;
		else if (!ft_strcmp(temp->input, "<<"))
			temp->type = NODE_HERE;
		else if (!ft_strcmp(temp->input, ">"))
			temp->type = NODE_OUT;
		else if (!ft_strcmp(temp->input, "<"))
			temp->type = NODE_IN;
		else
			temp->type = NODE_WORD;
		temp = temp->next;
	}
	//print_list(*tokens);
}

void	handle_input(char *prompt)
{
	char		**array;
	int			i;
	t_tokens	*tokens;

	i = 0;
	tokens = NULL;
	array = ft_split(prompt, ' ');
	while (array[i])
	{
		list_add(&tokens, array[i]);
		if (!ft_strcmp(array[i], "pwd"))
			pwd();
		i++;
	}
	add_type(&tokens);
	free_array(array, i);
	free_list(tokens);
}
