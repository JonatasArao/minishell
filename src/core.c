/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 00:46:52 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 04:31:07 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_msh	*get_minishell(void)
{
	static t_msh	msh;

	return (&msh);
}

void	init_minishell(t_msh *msh, char **envp)
{
	msh->env_vars = extract_env_vars(envp);
	msh->tokens = NULL;
	msh->commands = NULL;
	msh->input = NULL;
	msh->saved_fd[0] = -1;
	msh->saved_fd[1] = -1;
	msh->last_status = 0;
	signal(SIGINT, sigint_action);
	signal(SIGQUIT, SIG_IGN);
}

void	free_minishell_loop(t_msh *msh)
{
	if (msh->tokens)
		ft_lstclear(&msh->tokens, free);
	if (msh->commands)
		ft_lstclear(&msh->commands, free_command);
	if (msh->input)
		ft_delpointer((void **) &msh->input);
}

void	destroy_minishell(void)
{
	t_msh	*msh;

	msh = get_minishell();
	rl_clear_history();
	free_minishell_loop(msh);
	if (msh->env_vars)
		ft_lstclear(&msh->env_vars, free_env_var);
	if (msh->saved_fd[0] != -1)
	{
		close(msh->saved_fd[0]);
		close(STDIN_FILENO);
	}
	if (msh->saved_fd[1] != -1)
	{
		close(msh->saved_fd[1]);
		close(STDOUT_FILENO);
	}
}
