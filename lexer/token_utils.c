/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:23 by mefische          #+#    #+#             */
/*   Updated: 2026/03/20 12:29:22 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quote_start(char *prompt, t_token_state *state)
{
	state->in_quote = 1;
	state->quote_char = prompt[state->i];
	state->i++;
}

void	handle_quote_end(t_token_state *state)
{
	state->in_quote = 0;
	state->i++;
}

void	handle_space(t_tokens **tokens, char *prompt, t_token_state *state)
{
	if (state->i > state->start)
		add_token(tokens, ft_substr(prompt, state->start,
				state->i - state->start), NODE_UNKNOWN);
	while (prompt[state->i] == ' ')
		state->i++;
	state->start = state->i;
}

/* If is_redir_name = 1 I will skip later */
void	add_redir_info(t_tokens *token)
{
	if (token->next)
	{
		token->redir_file = ft_strdup(token->next->input);
		token->next->is_redir_name = 1;
	}
}

void	set_quote_type(t_tokens *token)
{
	int	len;

	if (token->type != NODE_DOUBLE_QUOTE && token->type != NODE_SINGLE_QUOTE)
		return ;
	len = ft_strlen(token->input);
	if (len >= 2 && token->input[0] == token->input[len - 1]
		&& (token->input[0] == '"' || token->input[0] == '\''))
		token->type = NODE_WORD;
}
