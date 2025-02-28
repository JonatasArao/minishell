/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:20:24 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/28 18:21:25 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_pipeline(pid_t last_pid, int num_commands)
{
	pid_t	pid;
	int		pid_status;
	int		last_status;

	last_status = 0;
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

int	execute_pipeline(t_minish *msh, int num_commands)
{
	t_list		*cmd_list;
	t_command	*cmd;
	int			last_status;

	cmd_list = msh->commands;
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
	return (last_status);
}

int	wait_single(pid_t pid)
{
	int	pid_status;
	int	status;

	status = 1;
	waitpid(pid, &pid_status, 0);
	if (WIFEXITED(pid_status))
		status = WEXITSTATUS(pid_status);
	else if (WIFSIGNALED(pid_status))
		status = WTERMSIG(pid_status) + 128;
	return (status);
}

int	execute_single(t_minish *msh)
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
		if (launch_process(msh, msh->commands, launch_executable))
			status = wait_single(cmd->pid);
	}
	return (status);
}

int	execute_commands(t_minish *msh)
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
