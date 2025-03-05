/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:33:45 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 02:52:17 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_var_end(const char *token)
{
	static char	quote_char;
	size_t		end;

	if (token[0] == '\0')
		return (0);
	end = 0;
	if (!quote_char && ft_strchr("\"'", token[end]))
		quote_char = token[end++];
	while (token[end] && (!ft_strchr("\"'", token[end]) || quote_char))
	{
		if (token[end] == quote_char || (token[0] == '$'
				&& (ft_isspace(token[end])
					|| ft_strchr("\"';()[]{}+-*/=", token[end])
					|| (end > 0 && token[end - 1] == '?'))))
			break ;
		if (quote_char != '\'' && token[end] == '$' && end != 0)
			return (end);
		end++;
	}
	if (token[0] != '$' && token[end] && token[end] == quote_char)
	{
		quote_char = 0;
		end++;
	}
	return (end);
}

static char	*get_next_var(char const *token)
{
	char			*word;
	unsigned int	start;
	size_t			end;

	start = 0;
	end = get_var_end(token);
	if (start == end)
		return (NULL);
	word = ft_substr(token, start, end - start);
	if (!word)
		return (NULL);
	return (word);
}

t_list	*extract_vars(const char *token)
{
	t_list	*head;
	t_list	*current;
	char	*word;

	head = NULL;
	while (*token)
	{
		word = get_next_var(token);
		if (!word && head)
			ft_lstclear(&head, free);
		if (!word)
			break ;
		token += ft_strlen(word);
		current = ft_lstnew((void *) word);
		if (head)
			ft_lstadd_back(&head, current);
		else
			head = current;
	}
	return (head);
}
