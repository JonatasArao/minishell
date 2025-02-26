/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:45:44 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/26 07:49:08 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_var(void *env_var_ptr)
{
	t_env_var	*env_var;

	env_var = (t_env_var *) env_var_ptr;
	if (env_var)
	{
		if (env_var->key)
			free(env_var->key);
		if (env_var->value)
			free(env_var->value);
		free(env_var);
	}
}

t_env_var	*get_env_var(t_list *env, const char *key)
{
	t_env_var	*current;
	size_t		key_len;

	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	while (env)
	{
		current = (t_env_var *) env->content;
		if (current->key && ft_strncmp(current->key, key, key_len) == 0
			&& current->key[key_len] == '\0')
			return (current);
		env = env->next;
	}
	return (NULL);
}

int	lstadd_env_var(t_list **env, const char *key, const char *value)
{
	t_env_var	*new_env;
	t_list		*new_node;

	if (!key || !(*key))
		return (0);
	new_env = ft_calloc(1, sizeof(t_env_var));
	if (!new_env)
		return (0);
	new_env->key = ft_strdup(key);
	if (value)
		new_env->value = ft_strdup(value);
	if (!new_env->key)
	{
		free_env_var(new_env);
		return (0);
	}
	new_node = ft_lstnew(new_env);
	if (!new_node)
	{
		free_env_var(new_env);
		return (0);
	}
	ft_lstadd_front(env, new_node);
	return (1);
}

int	lstset_env_var(t_list **env, const char *key, const char *value)
{
	t_env_var	*env_var;

	env_var = get_env_var(*env, key);
	if (!env_var)
		return (lstadd_env_var(env, key, value));
	free(env_var->value);
	env_var->value = ft_strdup(value);
	if (!env_var->value)
		return (0);
	return (1);
}

int	lstrm_env_var(t_list **env, const char *key)
{
	t_env_var	*target_var;

	target_var = get_env_var(*env, key);
	if (!target_var)
		return (0);
	if (!ft_lstrm(env, target_var, free_env_var))
		return (0);
	return (1);
}
