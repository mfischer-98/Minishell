/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:59:42 by mefische          #+#    #+#             */
/*   Updated: 2025/10/03 11:59:42 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	debug_tokens(t_tokens *tokens)
{
    int index = 0;
    
    printf("=== TOKEN DEBUG ===\n");
    while (tokens)
    {
        printf("TOKEN[%d]: '%s' (type=%d)\n", 
               index++, 
               tokens->input ? tokens->input : "NULL", 
               tokens->type);
        tokens = tokens->next;
    }
    printf("===================\n");
}

int main (int argc, char **argv, char **envp)
{
	t_mshell_data	*data;
	char			*prompt;
	
	(void)argc;
	(void)argv;
	print_banner();
	initialize(&data, envp);
	while (1)
	{
		prompt = readline("\x1b[32mminishell\x1b[0m> ");
		if (!prompt)
			exit(0);
		if (!check_exit(prompt))
			exit(0); //function to check exit status
		create_tokens(prompt, &data->tokens);
		debug_tokens(data->tokens);
		check_command(data);
		add_history(prompt);
		free_list(data->tokens); //fazer funcao free_data
		data->tokens = NULL;
	}
}
