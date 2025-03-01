/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:52:14 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/01 04:04:23 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_heredoc_var_end(const char *line)
{
	size_t		end;

	if (line[0] == '\0')
		return (0);
	end = 0;
	while (line[end])
	{
		if (line[0] == '$' && (ft_isspace(line[end])
				|| ft_strchr("\"';()[]{}+-*/=", line[end])
				|| (end > 0 && line[end - 1] == '?')))
			break ;
		if (line[end] == '$' && end != 0)
			return (end);
		end++;
	}
	return (end);
}

char	*get_next_heredoc_var(char const *token)
{
	char			*word;
	unsigned int	start;
	size_t			end;

	start = 0;
	end = get_heredoc_var_end(token);
	if (start == end)
		return (NULL);
	word = ft_substr(token, start, end - start);
	if (!word)
		return (NULL);
	return (word);
}

t_list	*extract_heredoc_vars(const char *token)
{
	t_list	*head;
	t_list	*current;
	char	*word;

	head = NULL;
	while (*token)
	{
		word = get_next_heredoc_var(token);
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

int	expand_heredoc_var(t_list *env, int last_status, char **var)
{
	char	*new_value;
	char	*content;

	new_value = NULL;
	content = (char *)(*var);
	if (content[0] == '$' && content[1] != '\0')
	{
		new_value = get_var_value(env, last_status, content + 1);
		if (!new_value)
			new_value = ft_strdup("");
		if (!new_value)
			return (0);
	}
	else
		return (1);
	free(*var);
	*var = new_value;
	return (1);
}

char	*expand_heredoc(t_list *env, int last_status, char *line)
{
	t_list	*var_list;
	t_list	*current;
	char	*new_line;
	char	**current_var;

	var_list = extract_heredoc_vars(line);
	if (!var_list)
		return (NULL);
	current = var_list;
	while (current)
	{
		current_var = (char **)&current->content;
		if (!expand_heredoc_var(env, last_status, current_var))
		{
			ft_lstclear(&var_list, free);
			return (NULL);
		}
		current = current->next;
	}
	new_line = concat_vars(var_list);
	ft_lstclear(&var_list, free);
	if (!new_line)
		return (NULL);
	return (new_line);
}
