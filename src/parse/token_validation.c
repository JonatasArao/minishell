/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:48:53 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/10 21:45:38 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_quotes(const char *s)
{
	char	quote_char;
	int		quote_count;

	quote_char = 0;
	quote_count = 0;
	while (*s)
	{
		if ((*s == '\'' || *s == '"') && !quote_char)
		{
			quote_char = *s;
			quote_count++;
		}
		else if (*s == quote_char)
		{
			quote_char = 0;
			quote_count++;
		}
		s++;
	}
	if (quote_count % 2 == 0)
		return (1);
	printf("minishell: unexpected EOF for `%c'\n", quote_char);
	return (0);
}

int	handle_syntax_error(const char *token, const char *next_token)
{
	if (next_token)
		printf("minishell: syntax error near token `%s'\n",
			next_token);
	else
		printf("minishell: syntax error near token `%s'\n", token);
	return (0);
}

int	is_token_list_valid(t_list *tokens)
{
	t_list	*token_node;
	char	*token;
	char	*next_token;

	if (!tokens)
		return (0);
	token_node = tokens;
	token = (char *)token_node->content;
	if (is_pipe(token))
		return (handle_syntax_error(token, NULL));
	while (token_node)
	{
		token = (char *)token_node->content;
		if (token_node->next)
			next_token = (char *)token_node->next->content;
		else
			next_token = NULL;
		if (((is_redirection(token) || is_pipe(token))
				&& (next_token == NULL || is_pipe(next_token)))
			|| (is_redirection(token) && is_redirection(next_token)))
			return (handle_syntax_error(token, next_token));
		token_node = token_node->next;
	}
	return (1);
}
