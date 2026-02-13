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

int	main(int argc, char **argv, char **envp)
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
		create_tokens(prompt, &data->tokens);
		executor(data);
		add_history(prompt);
		free_list(data->tokens);
		data->tokens = NULL;
	}
}
