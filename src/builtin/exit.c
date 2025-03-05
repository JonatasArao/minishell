/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 02:39:32 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 04:31:35 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_exit_numeric_arg(char *arg)
{
	if (arg[0] == '+' || arg[0] == '-')
		arg++;
	if (!ft_strall(arg, ft_isdigit))
	{
		ft_putendl_fd("minishell: exit: numeric argument required", 2);
		return (0);
	}
	return (1);
}

int	msh_exit(t_msh *msh, t_command *cmd)
{
	t_list	*args;
	char	*status;
	int		status_num;
	int		arg_count;

	args = cmd->arguments->next;
	arg_count = ft_lstsize(args);
	if (args)
	{
		status = (char *)args->content;
		if (!is_exit_numeric_arg(status))
			return (2);
		if (arg_count > 1)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		status_num = ft_atoi(status);
	}
	else
		status_num = msh->last_status;
	destroy_minishell();
	exit(status_num);
}
