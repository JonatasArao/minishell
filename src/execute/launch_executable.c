/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_executable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:19:45 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 00:52:54 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_execution(char *path, char **argv, char **envp)
{
	if (argv)
		ft_free_nt_matrix((void **) argv);
	if (envp)
		ft_free_nt_matrix((void **) envp);
	if (path)
		free(path);
}

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	if (S_ISDIR(path_stat.st_mode))
		return (1);
	return (0);
}

int	get_exec_error(char *path, char **argv)
{
	if (argv && argv[0])
		ft_putstr_fd(argv[0], 2);
	else
		ft_putstr_fd("minishell", 2);
	if (!path)
	{
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	if (is_directory(path))
	{
		ft_putendl_fd(": is a directory", 2);
		return (126);
	}
	if (access(path, F_OK) != 0)
	{
		ft_putendl_fd(": no such file or directiory", 2);
		return (127);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putendl_fd(": Permission Denied", 2);
		return (126);
	}
	return (0);
}

int	launch_executable(t_msh *msh, t_command *cmd)
{
	char	**envp;
	char	**argv;
	char	*path;
	int		exit_code;

	envp = get_envp(msh->env_vars);
	argv = get_argv(cmd->arguments);
	if (!argv)
	{
		free_execution(NULL, argv, envp);
		return (0);
	}
	path = get_command_path(msh->env_vars, argv[0]);
	if (!path || execve(path, argv, envp) == -1)
	{
		exit_code = get_exec_error(path, argv);
		free_execution(path, argv, envp);
		return (exit_code);
	}
	return (0);
}
