/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:41 by mefische          #+#    #+#             */
/*   Updated: 2026/03/20 09:54:06 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		initialize(t_mshell_data **data, char **envp)
{
	*data = malloc(sizeof(t_mshell_data));
	if (!data)
	{
		perror("malloc");
		return ;
	}
	(*data)->tokens = NULL;
	(*data)->env_var = NULL;
	(*data)->expander = malloc(sizeof(t_expander));
	if (!(*data)->expander)
	{
		free(*data);
		return ;
	}
	(*data)->expander->result = NULL;
	(*data)->expander->i = 0;
	(*data)->expander->in_single = 0;
	(*data)->expander->in_double = 0;
	(*data)->exit_status = 0;
	create_envp(envp, *data);
}

void	init_expander(t_expander **expander)
{
	if (!*expander)
	{
		*expander = malloc(sizeof(t_expander));
		(*expander)->result = NULL;
		(*expander)->i = 0;
		(*expander)->in_single = 0;
		(*expander)->in_double = 0;
	}
	if ((*expander)->result)
		free((*expander)->result);
	(*expander)->result = ft_strdup("");
	(*expander)->i = 0;
	(*expander)->in_single = 0;
	(*expander)->in_double = 0;

}

t_token_state	*init_state(void)
{
	t_token_state	*state;

	state = malloc (sizeof(t_token_state));
	if (!state)
		return (perror("malloc"), NULL);
	state->i = 0;
	state->start = 0;
	state->in_quote = 0;
	state->quote_char = 0;
	return (state);
}

char	*get_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*prompt;
	char	*temp;
	char	*user_pos;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (ft_strdup("\x1b[32mminishell\x1b[0m> "));
	}
	user_pos = ft_strnstr(cwd, "User", ft_strlen(cwd));
	if (user_pos == NULL)
		user_pos = cwd;
	temp = ft_strjoin("\x1b[32m", user_pos);
	if (!temp)
		return (ft_strdup("\x1b[32mminishell\x1b[0m> "));
	prompt = ft_strjoin(temp, "\x1b[0m> ");
	free(temp);
	if (!prompt)
		return (ft_strdup("\x1b[32mminishell\x1b[0m> "));
	return (prompt);
}
