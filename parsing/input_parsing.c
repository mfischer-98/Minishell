/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:12 by mefische          #+#    #+#             */
/*   Updated: 2025/10/07 15:15:50 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_type(t_commands *commands)
{
	char	*temp;

	temp = *commands;
	while (temp->next != NULL)
	{
		if (temp->input == '|')
			temp->type = ...
		if (temp->input == '"')
			temp->type = ...
		if (temp->input == )
			temp->type = ...
		if (temp->input == ...)
			temp->type = ...
		if (temp->input == ...)
			temp->type = ...
		temp = temp->next;
	}

}

void	handle_input(char *prompt)
{
	char	**array;
	int		i;
	t_commands	*commands;

	i = 0;
	commands = NULL;
	array = ft_split(prompt, ' ');
	while (array[i])
	{
		list_add(&commands, array[i]);
		i++;
	}
	free_array(array, i);
	add_type(commands);	
	//free_list(commands);
}
