/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:00:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/26 10:31:16 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_export_syntax(char *arg, char *equal_ptr)
{
	char	*key;

	if (ft_isalpha(*arg) || *arg == '_')
	{
		key = arg;
		while ((!equal_ptr || key < equal_ptr)
			&& (ft_isalnum(*key) || *key == '_'))
			key++;
		if (key == equal_ptr || *key == '\0')
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
	char	*equal_ptr;
	char	*key;
	char	*value;

	args = cmd->arguments->next;
	if (!args)
		return (msh_env(*env, NULL));
	while (args)
	{
		current_arg = (char *)args->content;
		equal_ptr = ft_strchr(current_arg, '=');
		if (!is_valid_export_syntax(current_arg, equal_ptr))
			return (1);
		key = ft_strtok_r(current_arg, "=", &value);
		if (!equal_ptr)
			value = NULL;
		if (value && !lstset_env_var(env, key, value))
			return (1);
		args = args->next;
	}
	return (0);
}
