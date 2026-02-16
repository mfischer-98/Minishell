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

static void	update_sig_status(t_mshell_data *data)
{
	data->exit_status = g_signal;
	g_signal = 0;
}

static void	main_loop(t_mshell_data *data)
{
	char	*prompt;

	while (1)
	{
		if (g_signal != 0)
			update_sig_status(data);
		prompt = readline("\x1b[32mminishell\x1b[0m> ");
		if (!prompt)
			break ;
		if (g_signal != 0) //for ctrl+C during typing
			update_sig_status(data);
		add_history(prompt);
		create_tokens(prompt, &data->tokens);
		free(prompt);
		sig_init_exec(); //silent handler caller before exec
		executor(data);
		if (g_signal != 0)
			update_sig_status(data);
		sig_init(); //restore ctrl+c interactive
		free_list(data->tokens);
		data->tokens = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mshell_data	*data;

	(void)argc;
	(void)argv;
	print_banner();
	initialize(&data, envp);
	rl_catch_signals = 0; //use our signal handlers
	sig_init();
	main_loop(data);
	rl_clear_history();
	exit(data->exit_status);
}
