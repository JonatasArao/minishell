/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/04 05:07:40 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_minishell_loop(t_minish *msh)
{
	if (msh->tokens)
		ft_lstclear(&msh->tokens, free);
	if (msh->commands)
		ft_lstclear(&msh->commands, free_command);
	if (msh->input)
		ft_delpointer((void **) &msh->input);
}

void	init_minishell(t_minish *msh, char **envp)
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

void	destroy_minishell(t_minish *msh)
{
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

int	process_input(t_minish *msh)
{
	msh->tokens = extract_tokens(msh->input);
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
	t_minish	msh;

	(void) argc;
	(void) argv;
	init_minishell(&msh, envp);
	if (!msh.env_vars)
		return (1);
	while (1)
	{
		msh.input = readline("Minishell $ ");
		if (msh.input == NULL)
			break ;
		if (process_input(&msh))
			msh.last_status = execute_commands(&msh);
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free_minishell_loop(&msh);
	}
	ft_putstr_fd("exit\n", 1);
	destroy_minishell(&msh);
	return (0);
}
