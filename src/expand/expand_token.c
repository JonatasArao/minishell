/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:53:33 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 15:06:31 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(t_list *env, int last_status, const char *key)
{
	t_env_var	*env_var;
	char		*result;

	if (!key || !(*key))
		return (NULL);
	if (key[0] == '?' && key[1] == '\0')
		result = ft_itoa(last_status);
	else
	{
		env_var = get_env_var(env, key);
		if (!env_var || (env_var && env_var->value && !(*env_var->value)))
			result = NULL;
		else
			result = ft_strdup(env_var->value);
	}
	if (!result)
		return (NULL);
	return (result);
}

char	*expand_quotes(char *s)
{
	char	*quote;
	char	*double_quote;
	char	*new_value;

	new_value = NULL;
	quote = ft_strrchr(s, '\'');
	double_quote = ft_strrchr(s, '"');
	if (double_quote && (!quote || double_quote > quote
			|| (ft_strncmp(s, "\"'", 2) == 0 && s[2] == '\0')))
		new_value = ft_strtrim(s, "\"");
	else if (quote && (!double_quote || quote > double_quote))
		new_value = ft_strtrim(s, "'");
	else
		return (s);
	if (!new_value)
		return (NULL);
	return (new_value);
}

int	expand_var(t_list *env, int last_status, char **var)
{
	char	*new_value;
	char	*content;

	new_value = NULL;
	content = (char *)(*var);
	if (content[0] == '$' && content[1] != '\0')
		new_value = get_var_value(env, last_status, content + 1);
	else
	{
		new_value = expand_quotes(content);
		if (new_value == content)
			return (1);
		if (!new_value)
			return (0);
	}
	free(*var);
	*var = new_value;
	return (1);
}

char	*expand_token(t_list *env, int last_status, char *token)
{
	t_list	*var_list;
	t_list	*current;
	char	*new_token;
	char	**current_var;

	var_list = extract_vars(token);
	if (!var_list)
		return (NULL);
	current = var_list;
	while (current)
	{
		current_var = (char **)&current->content;
		if (!expand_var(env, last_status, current_var))
		{
			ft_lstclear(&var_list, free);
			return (NULL);
		}
		current = current->next;
	}
	new_token = concat_vars(var_list);
	ft_lstclear(&var_list, free);
	if (!new_token)
		return (NULL);
	return (new_token);
}
