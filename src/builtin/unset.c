/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 02:13:42 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 02:43:41 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_unset_syntax(char *arg)
{
	char	*key;

	key = arg;
	if (ft_isalpha(*arg) || *arg == '_')
	{
		while (key && (ft_isalnum(*key) || *key == '_'))
			key++;
		if (!(*key))
			return (1);
	}
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (0);
}

int	msh_unset(t_list **env, t_command *cmd)
{
	t_list	*args;
	char	*current_arg;

	args = cmd->arguments->next;
	if (!args)
		return (msh_env(*env, NULL));
	while (args)
	{
		current_arg = (char *)args->content;
		if (!is_valid_unset_syntax(current_arg))
			return (1);
		if (!lstrm_env_var(env, current_arg))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(current_arg, 2);
			ft_putendl_fd("': variable not found", 2);
		}
		args = args->next;
	}
	return (0);
}
