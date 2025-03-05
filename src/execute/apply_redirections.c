/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:16:27 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 00:52:54 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_fds(t_msh *msh)
{
	msh->saved_fd[0] = dup(STDIN_FILENO);
	if (msh->saved_fd[0] == -1)
	{
		perror("minishell: redirection:");
		return (-1);
	}
	msh->saved_fd[1] = dup(STDOUT_FILENO);
	if (msh->saved_fd[1] == -1)
	{
		perror("minishell: redirection:");
		close(msh->saved_fd[0]);
		return (-1);
	}
	return (1);
}

void	restore_fds(t_msh *msh)
{
	if (msh->saved_fd[0] != -1)
	{
		if (dup2(msh->saved_fd[0], STDIN_FILENO) == -1)
			perror("minishell: restore input:");
		if (close(msh->saved_fd[0]) == -1)
			perror("minishell: close input:");
		msh->saved_fd[0] = STDIN_FILENO;
	}
	if (msh->saved_fd[1] != -1)
	{
		if (dup2(msh->saved_fd[1], STDOUT_FILENO) == -1)
			perror("minishell: restore output:");
		if (close(msh->saved_fd[1]) == -1)
			perror("minishell: close output:");
		msh->saved_fd[1] = STDOUT_FILENO;
	}
}

int	apply_input_redirection(int input_fd)
{
	if (input_fd != -1)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: input redirection: ");
			return (0);
		}
		if (close(input_fd) == -1)
		{
			perror("minishell: input redirection: ");
			return (0);
		}
	}
	return (1);
}

int	apply_output_redirection(int output_fd)
{
	if (output_fd != -1)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: output redirection: ");
			return (0);
		}
		if (close(output_fd) == -1)
		{
			perror("minishell: output redirection: ");
			return (0);
		}
	}
	return (1);
}

int	apply_redirections(t_command *cmd)
{
	int	input_fd;
	int	output_fd;

	if (!open_redirections(cmd))
		return (0);
	input_fd = cmd->input_fd;
	output_fd = cmd->output_fd;
	if (!apply_input_redirection(input_fd))
		return (0);
	cmd->input_fd = -1;
	if (!apply_output_redirection(output_fd))
	{
		if (input_fd != -1)
			close(input_fd);
		return (0);
	}
	cmd->output_fd = -1;
	return (1);
}
