/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:33:54 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 02:51:57 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cd_path(t_list *env, t_command *cmd)
{
	t_env_var	*env_var;
	t_list		*args;
	char		*path;

	args = cmd->arguments->next;
	if (args)
		path = (char *)args->content;
	else
	{
		env_var = get_env_var(env, "HOME");
		if (!env_var)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (NULL);
		}
		path = env_var->value;
	}
	return (path);
}

static int	is_invalid_cd_arguments(t_list *args)
{
	int	arg_count;

	arg_count = ft_lstsize(args);
	if (arg_count > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (1);
	}
	return (0);
}

static char	*change_directory(char *path)
{
	char	*pwd;
	char	*error_message;

	if (chdir(path) != 0)
	{
		error_message = ft_strjoin("minishell: cd: ", path);
		if (!error_message)
		{
			perror(path);
			return (NULL);
		}
		perror(error_message);
		free(error_message);
		return (NULL);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("minishell: cd: getcwd");
		return (NULL);
	}
	return (pwd);
}

static int	update_pwd(t_list **env, char *oldpwd, char *pwd)
{
	if (!lstset_env_var(env, "PWD", pwd)
		|| !lstset_env_var(env, "OLDPWD", oldpwd))
	{
		free(oldpwd);
		free(pwd);
		return (0);
	}
	free(oldpwd);
	free(pwd);
	return (1);
}

int	msh_cd(t_list **env, t_command *cmd)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;

	if (is_invalid_cd_arguments(cmd->arguments))
		return (1);
	path = get_cd_path(*env, cmd);
	if (!path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	pwd = change_directory(path);
	if (!pwd)
	{
		free(oldpwd);
		return (1);
	}
	update_pwd(env, oldpwd, pwd);
	return (0);
}
