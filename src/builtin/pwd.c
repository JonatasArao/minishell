/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:12:26 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 00:14:26 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_pwd(t_command *cmd)
{
	t_list	*args;
	char	*cwd;
	int		arg_count;

	args = cmd->arguments;
	arg_count = ft_lstsize(args);
	if (arg_count > 1)
	{
		ft_putendl_fd("minishell: pwd: too many arguments", 2);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cmd)
	{
		perror("minishell: pwd: ");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
