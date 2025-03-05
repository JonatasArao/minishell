/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:52:03 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 00:52:54 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_heredoc(t_msh *msh, char *delim)
{
	int		heredoc_fd[2];

	if (pipe(heredoc_fd) == -1)
	{
		perror("minishell: heredoc:");
		return (0);
	}
	if (!capture_heredoc(msh, delim, heredoc_fd[1]))
	{
		close(heredoc_fd[1]);
		close(heredoc_fd[0]);
		return (-1);
	}
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}

int	apply_heredoc(t_msh *msh, t_command *cmd, t_redirection *redir)
{
	if (is_heredoc(redir->type))
	{
		if (cmd->heredoc_fd != -1)
			close(cmd->heredoc_fd);
		cmd->heredoc_fd = open_heredoc(msh, redir->target);
		if (cmd->heredoc_fd == -1)
			return (0);
	}
	return (1);
}

int	setup_heredocs(t_msh *msh)
{
	t_list			*cmd_list;
	t_list			*redir_list;
	t_command		*current_cmd;
	t_redirection	*redir;

	cmd_list = msh->commands;
	while (cmd_list)
	{
		current_cmd = (t_command *)cmd_list->content;
		redir_list = current_cmd->redirections;
		while (redir_list)
		{
			redir = (t_redirection *)redir_list->content;
			if (!apply_heredoc(msh, current_cmd, redir))
				return (0);
			redir_list = redir_list->next;
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}
