/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:47:49 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 01:09:29 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_env(t_list *env, t_command *cmd)
{
	t_list		*current_env;
	t_env_var	*env_var;
	int			arg_count;

	if (cmd)
	{
		arg_count = ft_lstsize(cmd->arguments);
		if (arg_count > 1)
		{
			ft_putendl_fd("minishell: env: too many arguments", 2);
			return (1);
		}
	}
	current_env = env;
	while (current_env)
	{
		env_var = (t_env_var *)current_env->content;
		printf("%s=%s\n", env_var->key, env_var->value);
		current_env = current_env->next;
	}
	return (0);
}
