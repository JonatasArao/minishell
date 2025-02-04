/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:26:59 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/04 22:01:50 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *token)
{
	if (ft_strncmp(token, ">>", 2) == 0
		|| ft_strncmp(token, "<<", 2) == 0
		|| ft_strncmp(token, ">", 1) == 0
		|| ft_strncmp(token, "<", 1) == 0)
		return (1);
	return (0);
}

void	parse_command(t_list *tokens)
{
	t_list	*current;
	char	*prev_token;
	char	*token;

	current = tokens;
	prev_token = NULL;
	while (current)
	{
		token = (char *)current->content;
		if (prev_token && is_redirection(prev_token))
			printf("Redirecionamento (%s) para (%s)\n", prev_token, token);
		else if (ft_strncmp(token, "|", 1) == 0)
			printf("Pipe: %s\n", token);
		else if (!is_redirection(token))
			printf("Argumento: %s\n", token);
		prev_token = token;
		current = current->next;
	}
}
