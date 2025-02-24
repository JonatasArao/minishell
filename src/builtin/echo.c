/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:28:42 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/24 20:44:57 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_no_newline(char *current_arg)
{
	if (ft_strncmp("-n", current_arg, 2) == 0 && current_arg[2] == '\0')
		return (1);
	return (0);
}

static int	parse_echo_flags(t_list **args)
{
	char	*current_arg;
	int		newline;

	if (!(*args))
		return (1);
	current_arg = (*args)->content;
	newline = 1;
	if (is_no_newline(current_arg))
		newline = 0;
	while (*args && !newline)
	{
		current_arg = (*args)->content;
		if (!is_no_newline(current_arg))
			break ;
		*args = (*args)->next;
	}
	return (newline);
}

int	msh_echo(t_command *cmd)
{
	t_list	*args;
	char	*current_arg;
	int		newline;

	args = cmd->arguments->next;
	newline = parse_echo_flags(&args);
	while (args)
	{
		current_arg = args->content;
		if (current_arg && *current_arg)
			ft_putstr_fd(current_arg, 1);
		if (args->next)
			ft_putchar_fd(' ', 1);
		args = args->next;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
