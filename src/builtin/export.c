/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:00:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 02:37:09 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_export_syntax(char *arg)
{
	char	*key;
	char	*equal_ptr;

	equal_ptr = ft_strchr(arg, '=');
	if (equal_ptr && (ft_isalpha(*arg) || *arg == '_'))
	{
		key = arg;
		while (key < equal_ptr && (ft_isalnum(*key) || *key == '_'))
			key++;
		if (key == equal_ptr)
			return (1);
	}
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (0);
}

int	msh_export(t_list **env, t_command *cmd)
{
	t_list	*args;
	char	*current_arg;
	char	*key;
	char	*value;

	args = cmd->arguments->next;
	if (!args)
		return (msh_env(*env, NULL));
	while (args)
	{
		current_arg = (char *)args->content;
		if (!is_valid_export_syntax(current_arg))
			return (1);
		key = ft_strtok_r(current_arg, "=", &value);
		if (!value)
			value = "";
		if (!lstset_env_var(env, key, value))
			return (1);
		args = args->next;
	}
	return (0);
}
