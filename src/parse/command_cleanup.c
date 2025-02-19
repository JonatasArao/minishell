/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 21:46:13 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/19 16:31:32 by jarao-de         ###   ########.fr       */
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
		if (cmd->input_redir)
			ft_lstclear(&cmd->input_redir, free_redirection);
		if (cmd->output_redir)
			ft_lstclear(&cmd->output_redir, free_redirection);
		free(cmd);
	}
}
