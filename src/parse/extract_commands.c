/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:26:59 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/01 06:21:37 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*alloc_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->arguments = NULL;
	cmd->redirections = NULL;
	cmd->pid = -1;
	cmd->input_fd = -1;
	cmd->output_fd = -1;
	cmd->heredoc_fd = -1;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

t_list	*parse_tokens(t_list *token_node, t_command *cmd)
{
	char	*token;
	char	*prev_token;
	int		status;

	prev_token = NULL;
	status = 1;
	while (token_node && status)
	{
		token = (char *)token_node->content;
		if (prev_token && is_redirection(prev_token))
			status = lstadd_redir(&cmd->redirections, prev_token, token);
		else if (prev_token && is_pipe(prev_token))
			break ;
		else if (!is_redirection(token) && !is_pipe(token))
			status = lstadd_str(&cmd->arguments, token);
		prev_token = token;
		token_node = token_node->next;
	}
	if (!status)
		free_command(cmd);
	return (token_node);
}

t_list	*extract_commands(t_list *tokens)
{
	t_list		*token_node;
	t_list		*cmd_list;
	t_list		*new_cmd_node;
	t_command	*new_cmd;

	token_node = tokens;
	cmd_list = NULL;
	while (token_node)
	{
		new_cmd = alloc_command();
		if (!new_cmd)
			break ;
		token_node = parse_tokens(token_node, new_cmd);
		if (!new_cmd)
			break ;
		new_cmd_node = ft_lstnew(new_cmd);
		if (!new_cmd_node)
			break ;
		ft_lstadd_back(&cmd_list, new_cmd_node);
	}
	if (!new_cmd || !new_cmd_node)
		ft_lstclear(&cmd_list, free_command);
	return (cmd_list);
}
