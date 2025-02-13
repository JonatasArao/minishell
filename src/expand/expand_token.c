/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:53:33 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/13 16:57:36 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(t_list *env, int last_status, const char *key)
{
	char		*value;
	char		*result;

	if (!key || !env)
		return (NULL);
	if (key[0] == '?' && key[1] == '\0')
		result = ft_itoa(last_status);
	else
	{
		value = get_env_var(env, key)->value;
		if (!value)
			result = ft_strdup("");
		else
			result = ft_strdup(value);
	}
	if (!result)
		return (NULL);
	return (result);
}

int	expand_var(t_list *env, int last_status, char **var)
{
	char	*new_value;
	char	*content;
	char	*quote;
	char	*double_quote;

	new_value = NULL;
	content = (char *)(*var);
	if (content[0] == '$')
		new_value = get_var_value(env, last_status, content + 1);
	else
	{
		quote = ft_strrchr(content, '\'');
		double_quote = ft_strrchr(content, '"');
		if (double_quote && (!quote || double_quote > quote
				|| (ft_strncmp(content, "\"'", 2) == 0 && content[3] == '\0')))
			new_value = ft_strtrim(content, "\"");
		else if (quote && (!double_quote || quote > double_quote))
			new_value = ft_strtrim(content, "'");
	}
	if (!new_value)
		return (0);
	free(*var);
	*var = new_value;
	return (1);
}

char	*concat_vars(t_list *vars)
{
	char	*token;
	size_t	len;
	t_list	*current;

	len = 0;
	current = vars;
	while (current)
	{
		len += strlen((char *)current->content);
		current = current->next;
	}
	token = (char *)malloc(len + 1);
	if (!token)
		return (NULL);
	token[0] = '\0';
	current = vars;
	while (current)
	{
		ft_strlcat(token, (char *)current->content, len + 1);
		current = current->next;
	}
	return (token);
}

char	*expand_token(t_list *env, int last_status, char *s)
{
	t_list	*var_list;
	t_list	*current;
	char	*new_tok;

	var_list = extract_vars(s);
	if (!var_list)
		return (NULL);
	current = var_list;
	while (current)
	{
		if (!expand_var(env, last_status, (char **)&current->content))
		{
			ft_lstclear(&var_list, free);
			return (NULL);
		}
		current = current->next;
	}
	new_tok = concat_vars(var_list);
	ft_lstclear(&var_list, free);
	if (!new_tok)
		return (NULL);
	return (new_tok);
}
