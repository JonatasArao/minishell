/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 14:49:02 by jarao-de         ###   ########.fr       */
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
	msh->last_status = 0;
}

void	destroy_minishell(t_minish *msh)
{
	rl_clear_history();
	free_minishell_loop(msh);
	if (msh->env_vars)
		ft_lstclear(&msh->env_vars, free_env_var);
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
		msh->commands = expand_commands(msh->env_vars,
				msh->last_status, msh->commands);
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
		msh.input = readline("$ ");
		if (msh.input == NULL)
			break ;
		if (process_input(&msh))
			msh.last_status = launch_builtin(&msh, msh.commands->content);
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free_minishell_loop(&msh);
	}
	destroy_minishell(&msh);
	return (0);
}
