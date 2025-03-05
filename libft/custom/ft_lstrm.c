/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:58:52 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 15:02:33 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstrm(t_list **list, void *content, void (*del)(void *))
{
	t_list	*current;
	t_list	*previous;

	current = *list;
	previous = NULL;
	while (current)
	{
		if (current->content == content)
		{
			if (previous)
				previous->next = current->next;
			else
				*list = current->next;
			ft_lstdelone(current, del);
			return (1);
		}
		previous = current;
		current = current->next;
	}
	return (0);
}
