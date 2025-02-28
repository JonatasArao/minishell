/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:46:13 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/28 15:07:55 by jarao-de         ###   ########.fr       */
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
		if (cmd->redirections)
			ft_lstclear(&cmd->redirections, free_redirection);
		if (cmd->input_fd != -1)
			close(cmd->input_fd);
		if (cmd->output_fd != -1)
			close(cmd->output_fd);
		if (cmd->pipe_fd[0] != -1)
			close(cmd->pipe_fd[0]);
		if (cmd->pipe_fd[1] != -1)
			close(cmd->pipe_fd[1]);
		free(cmd);
	}
}
