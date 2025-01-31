/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/01/31 14:21:23 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	show_tokens(t_list *tokens)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		printf("Token: %s\n", (char *)current->content);
		current = current->next;
	}
}

int	main(void)
{
	char	*input;
	t_list	*tokens;

	while (1)
	{
		input = readline("$ ");
		if (input == NULL)
			break ;
		tokens = extract_tokens(input);
		if (tokens)
		{
			add_history(input);
			show_tokens(tokens);
			ft_lstclear(&tokens, free);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
