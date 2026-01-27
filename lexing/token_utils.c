#include "../minishell.h"

void handle_quote_start(char *prompt, t_token_state *state)
{
	state->in_quote = 1;
	state->quote_char = prompt[state->i];
	state->i++;
}

void handle_quote_end(t_tokens **tokens, char *prompt, t_token_state *state)
{
	t_node_type	type;

	if (state->quote_char == '"')
		type = NODE_DOUBLE_QUOTE;
	else
		type = NODE_SINGLE_QUOTE;
	add_token(tokens, ft_substr(prompt, state->start, state->i - state->start + 1), type);
	state->in_quote = 0;
	state->i++;
	state->start = state->i;
}

void handle_space(t_tokens **tokens, char *prompt, t_token_state *state)
{
	if (state->i > state->start)
	{
		//condition
		add_token(tokens, ft_substr(prompt, state->start, state->i - state->start), NODE_UNKNOWN);
	}
	while (prompt[state->i] == ' ')
		state->i++;
	state->start = state->i;
}