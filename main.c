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

int main (int argc, char **argv, char **envp)
{
	t_tokens	*tokens;
	char		*prompt;

	(void)argc;
	(void)argv;
	print_banner();
	while (1)
	{
		tokens = NULL;
		prompt = readline("\x1b[32mmili_fisc@Mel\x1b[0m:~$> ");
		if (!prompt)
			exit(0);
		if (!check_exit(prompt))
			exit(0); //function to check exit status
		create_tokens(prompt, &tokens);
		check_command(&tokens, envp);
		add_history(prompt);
		free_list(tokens);
	}
}
