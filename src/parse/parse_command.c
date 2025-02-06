/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:26:59 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/06 16:53:32 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_command(t_list *tokens)
{
	t_list	*token_node;
	char	*prev_token;
	char	*token;

	if (!is_token_list_valid(tokens))
		return ;
	token_node = tokens;
	prev_token = NULL;
	while (token_node)
	{
		token = (char *)token_node->content;
		if (prev_token && is_redirection(prev_token))
			printf("Redirecionamento (%s) para (%s)\n", prev_token, token);
		else if (ft_strncmp(token, "|", 1) == 0)
			printf("Pipe: %s\n", token);
		else if (!is_redirection(token))
			printf("Argumento: %s\n", token);
		prev_token = token;
		token_node = token_node->next;
	}
}
