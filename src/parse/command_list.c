/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:43:00 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/11 16:05:12 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lstadd_str(t_list **head, const char *str)
{
	char	*new_str;
	t_list	*new_node;

	new_str = ft_strdup(str);
	if (!new_str)
		return (0);
	new_node = ft_lstnew(new_str);
	if (!new_node)
	{
		free(new_str);
		return (0);
	}
	ft_lstadd_back(head, new_node);
	return (1);
}

int	lstadd_redir(t_list **head, const char *type, const char *target)
{
	t_redirection	*new_redir;
	t_list			*new_node;

	if (!(*type) || !(*target))
		return (0);
	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return (0);
	new_redir->type = ft_strdup(type);
	new_redir->target = ft_strdup(target);
	if (!new_redir->type || !new_redir->target)
	{
		free_redirection(new_redir);
		return (0);
	}
	new_node = ft_lstnew(new_redir);
	if (!new_node)
	{
		free_redirection(new_redir);
		return (0);
	}
	ft_lstadd_back(head, new_node);
	return (1);
}
