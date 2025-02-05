/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/05 18:07:11 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_minish	msh;

	while (1)
	{
		msh.input = readline("$ ");
		if (msh.input == NULL)
			break ;
		if (strcmp(msh.input, "exit") == 0)
		{
			free(msh.input);
			break ;
		}
		msh.tokens = extract_tokens(msh.input);
		if (msh.tokens)
		{
			parse_command(msh.tokens);
			ft_lstclear(&msh.tokens, free);
		}
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free(msh.input);
	}
	rl_clear_history();
	return (0);
}
