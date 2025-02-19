/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/19 17:04:01 by jarao-de         ###   ########.fr       */
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

void	destroy_minishell(t_minish *msh)
{
	rl_clear_history();
	free_minishell_loop(msh);
	if (msh->env_vars)
		ft_lstclear(&msh->env_vars, free_env_var);
}

void	print_commands(t_list *commands)
{
	t_command		*cmd;
	t_list			*node;
	t_redirection	*redir;

	while (commands)
	{
		cmd = (t_command *)commands->content;
		node = cmd->arguments;
		while (node)
		{
			printf("Argument: %s\n", (char *)node->content);
			node = node->next;
		}
		node = cmd->input_redir;
		while (node)
		{
			redir = (t_redirection *)node->content;
			printf("Input Redirection: %s %s\n", redir->type, redir->target);
			node = node->next;
		}
		node = cmd->output_redir;
		while (node)
		{
			redir = (t_redirection *)node->content;
			printf("Output Redirection: %s %s\n", redir->type, redir->target);
			node = node->next;
		}
		printf("----------\n");
		commands = commands->next;
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
	msh.last_status = 0;
	msh.env_vars = extract_env_vars(envp);
	if (!msh.env_vars)
		return (1);
	while (1)
	{
		msh.input = readline("$ ");
		if (msh.input == NULL
			|| (ft_strncmp(msh.input, "exit", 4) == 0 && msh.input[4] == '\0'))
			break ;
		if (process_input(&msh))
			print_commands(msh.commands);
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free_minishell_loop(&msh);
	}
	destroy_minishell(&msh);
	return (0);
}
