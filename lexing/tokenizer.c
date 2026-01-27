#include "../minishell.h"

void handle_token_type(t_tokens **tokens, char *prompt, t_token_state *state)
{
	if (state->in_quote && prompt[state->i] == '=')
	{
		state->i++;
		return;
	}
	if (!state->in_quote && (prompt[state->i] == '"' || prompt[state->i] == '\''))
		handle_quote_start(prompt, state);
	else if (state->in_quote && prompt[state->i] == state->quote_char)
		handle_quote_end(tokens, prompt, state);
	else if (!state->in_quote && prompt[state->i] == ' ')
		handle_space(tokens, prompt, state);
	else
		state->i++;
}

void	create_tokens(char *prompt, t_tokens **tokens)
{
	t_token_state	state;
	t_node_type		type;

	if (!prompt)
		return ;
	state.i = 0;
	state.start = 0;
	state.in_quote = 0;
	state.quote_char = 0;
	while(prompt[state.i])
		handle_token_type(tokens, prompt, &state);
	if (state.i > state.start)
	{
		if (state.in_quote)
		{
			if (state.quote_char == '"')
				type = NODE_DOUBLE_QUOTE;
			else
				type = NODE_SINGLE_QUOTE;
		}
		else
			type = NODE_UNKNOWN;
		add_token(tokens, ft_substr(prompt, state.start, state.i - state.start), type);
	}
	add_type(tokens);
}

void	add_token(t_tokens **tokens, char *input, t_node_type type)
{
	t_tokens	*new_node;
	t_tokens	*node;

	new_node = malloc(sizeof(t_tokens));
	if (!new_node)
		return;
	new_node->input = input;
	new_node->type = type;
	new_node->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_node;
		return;
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
	//se o token tiver ", só acaba quando achar outras "
	while (temp != NULL)
	{
		if (!ft_strcmp(temp->input, "|"))
			temp->type = NODE_PIPE;
		else if (!ft_strcmp(temp->input, ">>"))
			temp->type = NODE_APPEND;
		else if (!ft_strcmp(temp->input, "<<"))
			temp->type = NODE_HERE;
		else if (!ft_strcmp(temp->input, ">"))
			temp->type = NODE_OUT;
		else if (!ft_strcmp(temp->input, "<"))
			temp->type = NODE_IN;
		else if (temp->type == NODE_UNKNOWN || temp->type == NODE_SINGLE_QUOTE || temp->type == NODE_DOUBLE_QUOTE) // aspas possivelmente nao palavra
			temp->type = NODE_WORD;
		temp = temp->next;
	}
}
