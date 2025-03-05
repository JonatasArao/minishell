/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:52:03 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 03:01:38 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_delim_quotes(char *str)
{
	char	*src;
	char	*dst;
	int		has_quote;

	if (!str)
		return (0);
	src = str;
	dst = str;
	has_quote = 0;
	while (*src)
	{
		if (*src != '\'' && *src != '\"')
		{
			*dst = *src;
			dst++;
		}
		else
			has_quote = 1;
		src++;
	}
	*dst = '\0';
	return (has_quote);
}

static int	is_delimiter(char *input, char *delim)
{
	int	delim_len;

	delim_len = ft_strlen(delim);
	if (ft_strncmp(input, delim, delim_len) == 0
		&& input[delim_len] == '\0')
		return (1);
	return (0);
}

static char	*process_input_line(t_msh *msh, char *input, int has_quote)
{
	char	*line;

	if (has_quote || !(*input))
	{
		line = ft_strdup(input);
		if (!line)
			return (NULL);
	}
	else
	{
		line = expand_heredoc(msh->env_vars, msh->last_status, input);
		if (!line)
			return (NULL);
	}
	return (line);
}

int	capture_heredoc(t_msh *msh, char *delim, int heredoc_fd)
{
	char	*line;
	int		has_quote;

	has_quote = remove_delim_quotes(delim);
	while (1)
	{
		msh->input = readline("> ");
		if (!msh->input || is_delimiter(msh->input, delim))
		{
			if (!msh->input)
				ft_putchar_fd('\n', 1);
			else
				ft_delpointer((void **) &msh->input);
			break ;
		}
		line = process_input_line(msh, msh->input, has_quote);
		ft_delpointer((void **) &msh->input);
		if (!line)
			return (0);
		ft_putendl_fd(line, heredoc_fd);
		free(line);
	}
	return (1);
}
