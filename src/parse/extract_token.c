/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:46:55 by jarao-de          #+#    #+#             */
/*   Updated: 2025/01/31 12:01:36 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_token_end(const char *s, unsigned int start)
{
	size_t	end;
	char	quote_char;

	if (s[start] == '\0' || s[start] == ' ' || start >= ft_strlen(s))
		return (start);
	end = start;
	quote_char = 0;
	if (!ft_strncmp("<<", &s[start], 2) || !ft_strncmp(">>", &s[start], 2))
		end += 2;
	else if (ft_strchr(">|<", s[start]))
		end += 1;
	else
	{
		while (s[end] && ((!ft_isspace(s[end]) && !ft_strchr(">|<", s[end]))
				|| quote_char))
		{
			if ((s[end] == '\'' || s[end] == '"') && !quote_char)
				quote_char = s[end];
			else if (s[end] == quote_char)
				quote_char = 0;
			end++;
		}
	}
	return (end);
}

char	*get_next_token(const char *s)
{
	char			*token;
	unsigned int	start;
	size_t			end;

	start = 0;
	while (s[start] && ft_isspace(s[start]))
		start++;
	end = get_token_end(s, start);
	if (start == end)
		return (NULL);
	token = ft_substr(s, start, end - start);
	if (!token)
		return (NULL);
	return (token);
}

t_list	*extract_token(const char *s)
{
	t_list	*head;
	t_list	*current;
	char	*word;
	int		i;

	i = 0;
	head = NULL;
	while (s[i])
	{
		word = get_next_token(&s[i]);
		if (!word)
		{
			ft_lstclear(&head, free);
			break ;
		}
		i += ft_strlen(word);
		while (s[i] && ft_isspace(s[i]))
			i++;
		current = ft_lstnew((void *) word);
		if (head)
			ft_lstadd_back(&head, current);
		else
			head = current;
	}
	return (head);
}
