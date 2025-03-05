/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:18:59 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 02:59:07 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_full_path(char *token, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(token, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	return (full_path);
}

static char	*search_path(char *path, char *cmd)
{
	char	*token;
	char	*full_path;

	token = ft_strtok(path, ":");
	while (token)
	{
		full_path = get_full_path(token, cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		token = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*get_command_path(t_list *env, char *cmd)
{
	t_env_var	*path_env;
	char		*path;
	char		*full_path;

	if (cmd[0] == '/' || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 2) == 0)
	{
		full_path = ft_strdup(cmd);
		if (!full_path)
			return (NULL);
		return (full_path);
	}
	path_env = get_env_var(env, "PATH");
	if (!path_env || (path_env && !path_env->value))
		return (NULL);
	path = ft_strdup(path_env->value);
	if (!path)
		return (NULL);
	full_path = search_path(path, cmd);
	free(path);
	if (full_path)
		return (full_path);
	return (NULL);
}
