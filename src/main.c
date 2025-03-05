/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 01:29:12 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input(t_msh *msh)
{
	msh->tokens = extract_tokens(msh->input);
	ft_delpointer((void **) &msh->input);
	if (!msh->tokens || !is_token_list_valid(msh->tokens))
	{
		if (msh->tokens)
			ft_lstclear(&msh->tokens, free);
		return (0);
	}
	msh->commands = extract_commands(msh->tokens);
	if (msh->commands)
	{
		msh->commands = expand_commands(msh->env_vars,
				msh->last_status, msh->commands);
		if (!setup_heredocs(msh))
			ft_lstclear(&msh->commands, free_command);
	}
	if (!msh->commands)
	{
		ft_lstclear(&msh->tokens, free);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	*msh;

	(void) argc;
	(void) argv;
	msh = get_minishell();
	init_minishell(msh, envp);
	if (!msh->env_vars)
		return (1);
	while (1)
	{
		msh->input = readline("Minishell $ ");
		if (msh->input == NULL)
			break ;
		if (!ft_strall(msh->input, ft_isspace))
			add_history(msh->input);
		if (process_input(msh))
			msh->last_status = execute_commands(msh);
		free_minishell_loop(msh);
	}
	ft_putstr_fd("exit\n", 1);
	destroy_minishell(msh);
	return (0);
}
