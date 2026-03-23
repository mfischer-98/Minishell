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

/* If word before redirect exists, have it as token
	- Add <, > << or >> token with correct type
	- Advance i and reset start to after the redirect symbol */
static void	handle_redirect(t_tokens **tokens, char *prompt,
	t_token_state *state, int is_double)
{
	if (state->i > state->start)
		add_token(tokens, ft_substr(prompt, state->start,
				state->i - state->start), NODE_UNKNOWN);
	if (prompt[state->i] == '<')
	{
		if (is_double)
		{
			add_token(tokens, ft_strdup("<<"), NODE_HERE);
			state->i++;
		}
		else
			add_token(tokens, ft_strdup("<"), NODE_IN);
	}
	else if (prompt[state->i] == '>')
	{
		if (is_double)
		{
			add_token(tokens, ft_strdup(">>"), NODE_APPEND);
			state->i++;
		}
		else
			add_token(tokens, ft_strdup(">"), NODE_OUT);
	}
	state->i++;
	state->start = state->i;
}

/* Reads current char and decides what to do:
	- redirect, pipe, quote open/close, space or just i++
	- skips = inside quotes to be plain character */
void	handle_tok_type(t_tokens **tokens, char *prompt, t_token_state *state)
{
	if (!state->in_quote && prompt[state->i] == '<')
		return (handle_redirect(tokens, prompt, state,
				prompt[state->i + 1] == '<'), (void)0);
	if (!state->in_quote && prompt[state->i] == '>')
		return (handle_redirect(tokens, prompt, state,
				prompt[state->i + 1] == '>'), (void)0);
	if (!state->in_quote && prompt[state->i] == '|')
	{
		if (state->i > state->start)
			add_token(tokens, ft_substr(prompt, state->start,
					state->i - state->start), NODE_UNKNOWN);
		add_token(tokens, ft_strdup("|"), NODE_PIPE);
		state->i++;
		return (state->start = state->i, (void)0);
	}
	if (state->in_quote && prompt[state->i] == '=')
		return (state->i++, (void)0);
	if (!state->in_quote && (prompt[state->i] == '"'
			|| prompt[state->i] == '\''))
		return (handle_quote_start(prompt, state), (void)0);
	if (state->in_quote && prompt[state->i] == state->quote_char)
		return (handle_quote_end(state), (void)0);
	if (!state->in_quote && prompt[state->i] == ' ')
		return (handle_space(tokens, prompt, state), (void)0);
	state->i++;
}

/* Entry point of lexer
	- walks the full input string char by char with handle_tok_type
	- assigns quote type if unclosed then calls add_type to finalize */
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

/* Create new token node, sets input, type and default fields,
   then appends it to the end of the token linked list */
void	add_token(t_tokens **tokens, char *input, t_node_type type)
{
	t_tokens	*new_node;
	t_tokens	*node;

	new_node = malloc(sizeof(t_tokens));
	if (!new_node)
		return (free(input), (void)0);
	new_node->input = input;
	new_node->type = type;
	new_node->redir_file = NULL;
	new_node->heredoc_fd = -1;
	new_node->quote_delim = 0;
	new_node->is_redir_name = 0;
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

/* Adds type to node
	- word if inside quotes
	- redir if is redir */
void	add_type(t_tokens **tokens)
{
	t_tokens	*temp;

	temp = *tokens;
	while (temp)
	{
		set_quote_type(temp);
		if (temp->type == NODE_UNKNOWN)
			temp->type = NODE_WORD;
		temp = temp->next;
	}
	temp = *tokens;
	while (temp)
	{
		if (temp->type == NODE_IN || temp->type == NODE_OUT
			|| temp->type == NODE_APPEND || temp->type == NODE_HERE)
			add_redir_info(temp);
		temp = temp->next;
	}
}
