/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:15:33 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 02:55:26 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_fd_open(int fd, char *target)
{
	char	*error_message;

	if (fd != -1)
		return (1);
	error_message = ft_strjoin("minishell: ", target);
	if (!error_message)
		perror("minishell: ");
	else
	{
		perror(error_message);
		free(error_message);
	}
	return (0);
}

static int	open_output_redirections(t_redirection *redir, int *output_fd)
{
	char	*type;
	int		flags;
	int		fd;

	type = redir->type;
	if (ft_strncmp(type, ">", 1) == 0 && type[1] == '\0')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (ft_strncmp(type, ">>", 2) == 0 && type[2] == '\0')
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (0);
	fd = open(redir->target, flags, 0644);
	if (!is_fd_open(fd, redir->target))
		return (0);
	*output_fd = fd;
	return (1);
}

static int	open_input_redirections(t_command *cmd, t_redirection *redir,
		int *input_fd)
{
	char	*type;
	int		fd;

	type = redir->type;
	if (ft_strncmp(type, "<", 1) == 0 && type[1] == '\0')
	{
		fd = open(redir->target, O_RDONLY, 0644);
		if (!is_fd_open(fd, redir->target))
			return (0);
		*input_fd = fd;
	}
	else if (ft_strncmp(type, "<<", 2) == 0 && type[2] == '\0')
	{
		fd = cmd->heredoc_fd;
		*input_fd = fd;
	}
	else
		return (0);
	return (1);
}

int	open_redirections(t_command *cmd)
{
	t_list			*redir_list;
	t_redirection	*redir;

	redir_list = cmd->redirections;
	while (redir_list)
	{
		redir = (t_redirection *)redir_list->content;
		if (is_input_redirection(redir->type))
		{
			if (cmd->input_fd != cmd->heredoc_fd && cmd->input_fd != -1)
				close(cmd->input_fd);
			if (!open_input_redirections(cmd, redir, &cmd->input_fd))
				return (0);
		}
		if (is_output_redirection(redir->type))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			if (!open_output_redirections(redir, &cmd->output_fd))
				return (0);
		}
		redir_list = redir_list->next;
	}
	return (1);
}
