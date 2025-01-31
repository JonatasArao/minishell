/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:46:55 by jarao-de          #+#    #+#             */
/*   Updated: 2025/01/31 09:59:00 by jarao-de         ###   ########.fr       */
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

char	*get_next_token(char const *s)
{
	char			*word;
	unsigned int	start;
	size_t			end;

	start = 0;
	while (s[start] && ft_isspace(s[start]))
		start++;
	end = get_token_end(s, start);
	if (start == end)
		return (NULL);
	word = ft_substr(s, start, end - start);
	if (!word)
		return (NULL);
	return (word);
}
