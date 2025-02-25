/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:05:53 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 15:06:00 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	varlst_len(t_list *vars)
{
	size_t	len;
	t_list	*current;

	len = 0;
	current = vars;
	while (current)
	{
		if (current->content)
			len += ft_strlen(current->content);
		current = current->next;
	}
	return (len);
}

char	*concat_vars(t_list *vars)
{
	size_t	len;
	t_list	*current;
	char	*token;

	if (!vars || (vars && !vars->content && !vars->next))
		return (NULL);
	len = varlst_len(vars);
	token = (char *)malloc(len + 1);
	if (!token)
		return (NULL);
	token[0] = '\0';
	current = vars;
	while (current)
	{
		if (current->content)
			ft_strlcat(token, current->content, len + 1);
		current = current->next;
	}
	return (token);
}
