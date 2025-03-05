/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:17:49 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 00:52:54 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_child_process(t_msh *msh, t_list *cmd_node,
			int input_fd, int exit_code)
{
	if (input_fd == STDIN_FILENO || !cmd_node->next)
		close(STDIN_FILENO);
	if (!cmd_node->next)
		close(STDOUT_FILENO);
	destroy_minishell(msh);
	exit(exit_code);
}

void	setup_child_pipes(t_msh *msh, t_list *cmd_node, int input_fd)
{
	t_command	*cmd;

	cmd = (t_command *)cmd_node->content;
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit_child_process(msh, cmd_node, input_fd, 1);
		}
		close(input_fd);
	}
	if (cmd_node->next)
	{
		close(cmd->pipe_fd[0]);
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit_child_process(msh, cmd_node, input_fd, 1);
		}
		close(cmd->pipe_fd[1]);
	}
}

void	child_process(t_msh *msh, t_list *cmd_node, int input_fd,
	int (*launcher)(t_msh *, t_command *))
{
	t_command	*cmd;
	int			exit_code;

	cmd = (t_command *)cmd_node->content;
	setup_child_pipes(msh, cmd_node, input_fd);
	if (!apply_redirections(cmd))
		exit_child_process(msh, cmd_node, input_fd, 1);
	exit_code = launcher(msh, cmd);
	exit_child_process(msh, cmd_node, input_fd, exit_code);
}

int	parent_process(t_list *cmd_node, pid_t pid, int input_fd)
{
	t_command	*cmd;

	cmd = (t_command *)cmd_node->content;
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (cmd_node->next)
	{
		close(cmd->pipe_fd[1]);
		input_fd = cmd->pipe_fd[0];
	}
	else
	{
		close(cmd->pipe_fd[0]);
		input_fd = STDIN_FILENO;
	}
	cmd->pid = pid;
	return (input_fd);
}

int	launch_process(t_msh *msh, t_list *cmd_node,
		int (*launcher)(t_msh *, t_command *))
{
	static int			input_fd = STDIN_FILENO;
	t_command			*cmd;
	pid_t				pid;

	cmd = (t_command *)cmd_node->content;
	if (pipe(cmd->pipe_fd) == -1)
	{
		perror("minishell: pipe:");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork:");
		exit(1);
	}
	if (pid == 0)
		child_process(msh, cmd_node, input_fd, launcher);
	else
		input_fd = parent_process(cmd_node, pid, input_fd);
	return (1);
}
