/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:26:59 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/08 22:22:29 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirection(void *redir_ptr)
{
	t_redirection	*redir;

	redir = (t_redirection *) redir_ptr;
	if (redir)
	{
		if (redir->target)
			free(redir->target);
		if (redir->type)
			free(redir->type);
		free(redir);
	}
}

void	free_command(void *cmd_ptr)
{
	t_command	*cmd;

	cmd = (t_command *) cmd_ptr;
	if (cmd)
	{
		if (cmd->arguments)
			ft_lstclear(&cmd->arguments, free);
		if (cmd->input_redirection)
			ft_lstclear(&cmd->input_redirection, free_redirection);
		if (cmd->output_redirection)
			ft_lstclear(&cmd->output_redirection, free_redirection);
		free(cmd);
	}
}

void	parse_command(t_list *tokens)
{
	t_list	*token_node;
	char	*prev_token;
	char	*token;

	token_node = tokens;
	prev_token = NULL;
	while (token_node)
	{
		token = (char *)token_node->content;
		if (prev_token && is_redirection(prev_token))
			printf("Redirecionamento (%s) para (%s)\n", prev_token, token);
		else if (ft_strncmp(token, "|", 1) == 0)
			printf("Pipe: %s\n", token);
		else if (!is_redirection(token))
			printf("Argumento: %s\n", token);
		prev_token = token;
		token_node = token_node->next;
	}
}
