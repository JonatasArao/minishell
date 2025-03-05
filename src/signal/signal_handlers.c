/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 03:19:15 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 02:09:25 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_action(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigint_process_action(int sig)
{
	if (sig == SIGINT)
		ft_putchar_fd('\n', 1);
}

void	sigint_heredoc_action(int sig)
{
	t_msh	*msh;

	msh = get_minishell();
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		destroy_minishell(msh);
		exit(EXIT_FAILURE);
	}
}
