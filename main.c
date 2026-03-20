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

void	update_sig_status(t_mshell_data *data)
{
	data->exit_status = g_signal;
	g_signal = 0;
}

static void	main_loop(t_mshell_data *data)
{
	char	*prompt;
	char	*readline_input;

	while (1)
	{
		if (g_signal != 0)
			update_sig_status(data);
		prompt = get_prompt();
		readline_input = readline(prompt);
		free(prompt);
		if (!readline_input)
			break ;
		if (g_signal != 0)
			update_sig_status(data);
		add_history(readline_input);
		create_tokens(readline_input, &data->tokens);
		free(readline_input);
		sig_init_exec();
		parser(data);
		if (g_signal != 0)
			update_sig_status(data);
		sig_init();
		free_loop(data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mshell_data	*data;
	int				exit_code;

	(void)argc;
	(void)argv;
	//print_banner();
	initialize(&data, envp);
	rl_catch_signals = 0;
	sig_init();
	main_loop(data);
	exit_code = data->exit_status;
	free_data(data);
	rl_clear_history();
	return (exit_code);
}

/* int	main(int argc, char **argv, char **envp)
{
	t_mshell_data	*data;
	int				exit_status;

	(void)argc;
	(void)argv;
	print_banner();
	initialize(&data, envp);
	rl_catch_signals = 0;
	sig_init();
	main_loop(data);
	exit_status = data->exit_status;
	free_data(data);
	rl_clear_history();
	return (exit_status);
} */