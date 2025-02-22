/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 07:18:15 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/22 00:47:50 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_string(const char *key, const char *value)
{
	char	*env_string;
	size_t	key_len;
	size_t	value_len;

	if (!key || !(*key) || !value)
		return (NULL);
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	env_string = (char *) malloc(key_len + value_len + 2);
	if (!env_string)
		return (NULL);
	ft_strlcpy(env_string, key, key_len + 1);
	ft_strlcat(env_string, "=", key_len + 2);
	ft_strlcat(env_string, value, key_len + value_len + 2);
	return (env_string);
}

char	**get_envp(t_list *env)
{
	t_list		*current;
	t_env_var	*env_var;
	char		**envp;
	int			len;
	int			i;

	len = ft_lstsize(env);
	if (!len)
		return (NULL);
	envp = (char **) malloc((len + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_var = (t_env_var *) current->content;
		envp[i] = create_env_string(env_var->key, env_var->value);
		if (!envp[i])
			return (ft_free_matrix((void **) envp, i));
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

t_list	*extract_env_vars(char **envp)
{
	t_list	*env;
	char	*saveptr;
	char	*key;
	char	*value;

	if (!envp)
		return (NULL);
	env = NULL;
	while (*envp != NULL)
	{
		key = ft_strtok_r(*envp, "=", &saveptr);
		value = saveptr;
		if (!value)
			value = "";
		if (*envp[0] == '=' || !lstadd_env_var(&env, key, value))
		{
			ft_lstclear(&env, free_env_var);
			return (NULL);
		}
		envp++;
	}
	return (env);
}
