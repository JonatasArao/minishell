/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:45:44 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/12 16:49:38 by jarao-de         ###   ########.fr       */
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

int	lstadd_env_var(t_list **head, const char *key, const char *value)
{
	t_env_var	*new_env;
	t_list		*new_node;

	if (!value || !key || !(*key))
		return (0);
	new_env = malloc(sizeof(t_env_var));
	if (!new_env)
		return (0);
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	if (!new_env->key || !new_env->value)
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
	ft_lstadd_front(head, new_node);
	return (1);
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
		if (current-> key && ft_strncmp(current->key, key, key_len) == 0
			&& current->key[key_len] == '\0')
			return (current);
		env = env->next;
	}
	return (NULL);
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
