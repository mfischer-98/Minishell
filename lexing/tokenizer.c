/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:26 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 09:21:26 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_tok_type(t_tokens **tokens, char *prompt, t_token_state *state)
{
	if (state->in_quote && prompt[state->i] == '=')
	{
		state->i++;
		return ;
	}
	if (!state->in_quote && (prompt[state->i] == '"'
			|| prompt[state->i] == '\''))
	{
		handle_quote_start(prompt, state);
		return ;
	}
	else if (state->in_quote && prompt[state->i] == state->quote_char)
	{
		handle_quote_end(tokens, prompt, state);
		return ;
	}
	else if (!state->in_quote && prompt[state->i] == ' ')
	{
		handle_space(tokens, prompt, state);
		return ;
	}
	else
		state->i++;
}

void	create_tokens(char *prompt, t_tokens **tokens)
{
	t_token_state	*state;
	t_node_type		type;

	if (!prompt)
		return ;
	state = init_state();
	while (prompt[state->i])
		handle_tok_type(tokens, prompt, state);
	if (state->i > state->start)
	{
		if (state->in_quote)
		{
			if (state->quote_char == '"')
				type = NODE_DOUBLE_QUOTE;
			else
				type = NODE_SINGLE_QUOTE;
		}
		else
			type = NODE_UNKNOWN;
		add_token(tokens, ft_substr(prompt, state->start,
				state->i - state->start), type);
	}
	add_type(tokens);
	free(state);
}

void	add_token(t_tokens **tokens, char *input, t_node_type type)
{
	t_tokens	*new_node;
	t_tokens	*node;

	new_node = malloc(sizeof(t_tokens));
	if (!new_node)
		return ;
	new_node->input = input;
	new_node->type = type;
	new_node->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_node;
		return ;
	}
	node = *tokens;
	while (node->next)
		node = node->next;
	node->next = new_node;
}

void	add_type(t_tokens **tokens)
{
	t_tokens	*temp;

	temp = *tokens;
	while (temp)
	{
		set_operator_type(temp);
		set_quote_type(temp);
		if (temp->type == NODE_UNKNOWN)
			temp->type = NODE_WORD;
		temp = temp->next;
	}
}
