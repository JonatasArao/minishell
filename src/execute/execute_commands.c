/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:20:24 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 03:01:06 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_pipeline(pid_t last_pid, int num_commands)
{
	pid_t	pid;
	int		pid_status;
	int		last_status;

	last_status = 0;
	pid_status = 0;
	while (num_commands > 0)
	{
		pid = waitpid(-1, &pid_status, 0);
		if (pid == -1)
		{
			perror("minishell: waitpid");
			return (1);
		}
		if (pid == last_pid)
		{
			if (WIFEXITED(pid_status))
				last_status = WEXITSTATUS(pid_status);
			else if (WIFSIGNALED(pid_status))
				last_status = WTERMSIG(pid_status) + 128;
		}
		num_commands--;
	}
	return (last_status);
}

static int	execute_pipeline(t_msh *msh, int num_commands)
{
	t_list		*cmd_list;
	t_command	*cmd;
	int			last_status;

	cmd_list = msh->commands;
	signal(SIGINT, sigint_process_action);
	while (cmd_list)
	{
		cmd = (t_command *)cmd_list->content;
		if (is_builtin(cmd))
			launch_process(msh, cmd_list, launch_builtin);
		else
			launch_process(msh, cmd_list, launch_executable);
		cmd_list = cmd_list->next;
	}
	last_status = wait_pipeline(cmd->pid, num_commands);
	signal(SIGINT, sigint_action);
	return (last_status);
}

static int	wait_single(pid_t pid)
{
	int	pid_status;
	int	status;

	status = 1;
	pid_status = 0;
	waitpid(pid, &pid_status, 0);
	if (WIFEXITED(pid_status))
		status = WEXITSTATUS(pid_status);
	else if (WIFSIGNALED(pid_status))
		status = WTERMSIG(pid_status) + 128;
	return (status);
}

static int	execute_single(t_msh *msh)
{
	t_command	*cmd;
	int			status;

	cmd = (t_command *)msh->commands->content;
	if (is_builtin(cmd))
	{
		if (!backup_fds(msh))
			return (1);
		if (!apply_redirections(cmd))
		{
			restore_fds(msh);
			return (1);
		}
		status = launch_builtin(msh, cmd);
		restore_fds(msh);
	}
	else
	{
		signal(SIGINT, sigint_process_action);
		if (launch_process(msh, msh->commands, launch_executable))
			status = wait_single(cmd->pid);
		signal(SIGINT, sigint_action);
	}
	return (status);
}

int	execute_commands(t_msh *msh)
{
	int	num_commands;
	int	status;

	num_commands = ft_lstsize(msh->commands);
	status = 0;
	if (num_commands == 1)
		status = execute_single(msh);
	else
		status = execute_pipeline(msh, num_commands);
	return (status);
}
