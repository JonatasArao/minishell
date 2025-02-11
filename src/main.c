/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/10 22:00:29 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arguments(t_list *arguments)
{
	t_list	*node;

	node = arguments;
	while (node)
	{
		printf("Argument: %s\n", (char *)node->content);
		node = node->next;
	}
}

void	print_redirections(t_list *redirections, const char *type)
{
	t_redirection	*redir;
	t_list			*node;

	node = redirections;
	while (node)
	{
		redir = (t_redirection *)node->content;
		printf("%s Redirection: %s %s\n", type, redir->type, redir->target);
		node = node->next;
	}
}

void	print_commands(t_list *commands)
{
	t_command	*cmd;

	while (commands)
	{
		cmd = (t_command *)commands->content;
		print_arguments(cmd->arguments);
		print_redirections(cmd->input_redir, "Input");
		print_redirections(cmd->output_redir, "Output");
		printf("----------\n");
		commands = commands->next;
	}
}

void	process_input(t_minish *msh)
{
	msh->tokens = extract_tokens(msh->input);
	if (msh->tokens && is_token_list_valid(msh->tokens))
	{
		msh->commands = extract_commands(msh->tokens);
		if (msh->commands)
		{
			print_commands(msh->commands);
			ft_lstclear(&msh->commands, free_command);
		}
	}
	if (msh->tokens)
		ft_lstclear(&msh->tokens, free);
}

int	main(void)
{
	t_minish	msh;

	while (1)
	{
		msh.input = readline("$ ");
		if (msh.input == NULL)
			break ;
		if (ft_strncmp(msh.input, "exit", 4) == 0 && msh.input[4] == '\0')
		{
			free(msh.input);
			break ;
		}
		process_input(&msh);
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free(msh.input);
	}
	rl_clear_history();
	return (0);
}
