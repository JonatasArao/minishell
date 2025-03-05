/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:52:03 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 02:08:41 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_heredoc_child(t_msh *msh, t_command *cmd, char *delim,
			int heredoc_fd[2])
{
	signal(SIGINT, sigint_heredoc_action);
	cmd->heredoc_fd = heredoc_fd[1];
	close(heredoc_fd[0]);
	if (!capture_heredoc(msh, delim, heredoc_fd[1]))
	{
		close(heredoc_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(heredoc_fd[1]);
	destroy_minishell(msh);
	exit(EXIT_SUCCESS);
}

int	process_heredoc_parent(t_command *cmd, int heredoc_fd[2], pid_t pid)
{
	int	status;

	close(heredoc_fd[1]);
	cmd->heredoc_fd = heredoc_fd[0];
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		close(heredoc_fd[0]);
		cmd->heredoc_fd = -1;
		return (0);
	}
	return (1);
}

int	open_heredoc(t_msh *msh, t_command *cmd, char *delim)
{
	int		heredoc_fd[2];
	pid_t	pid;

	if (pipe(heredoc_fd) == -1)
	{
		perror("minishell: heredoc:");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork:");
		close(heredoc_fd[0]);
		close(heredoc_fd[1]);
		return (0);
	}
	if (pid == 0)
		process_heredoc_child(msh, cmd, delim, heredoc_fd);
	else
		return (process_heredoc_parent(cmd, heredoc_fd, pid));
	return (1);
}

int	apply_heredoc(t_msh *msh, t_command *cmd, t_redirection *redir)
{
	signal(SIGINT, SIG_IGN);
	if (is_heredoc(redir->type))
	{
		if (cmd->heredoc_fd != -1)
			close(cmd->heredoc_fd);
		if (!open_heredoc(msh, cmd, redir->target))
			return (0);
	}
	signal(SIGINT, sigint_action);
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
