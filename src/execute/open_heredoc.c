/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:52:03 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/01 04:14:18 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_delim_quotes(char *str)
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

int	is_delimiter(char *input, char *delim)
{
	int	delim_len;

	delim_len = ft_strlen(delim);
	if (ft_strncmp(input, delim, delim_len) == 0
		&& input[delim_len] == '\0')
		return (1);
	return (0);
}

char	*process_input_line(t_minish *msh, char *input, int has_quote)
{
	char	*line;

	if (has_quote)
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

int	capture_heredoc(t_minish *msh, char *delim, int heredoc_fd)
{
	char	*input;
	char	*line;
	int		has_quote;

	has_quote = remove_delim_quotes(delim);
	while (1)
	{
		input = readline("> ");
		if (!input)
			return (0);
		if (is_delimiter(input, delim))
		{
			free(input);
			break ;
		}
		line = process_input_line(msh, input, has_quote);
		free(input);
		if (!line)
			return (0);
		ft_putendl_fd(line, heredoc_fd);
		free(line);
	}
	return (1);
}

int	open_heredoc(t_minish *msh, char *delim)
{
	int		heredoc_fd[2];
	int		status;

	if (pipe(heredoc_fd) == -1)
	{
		perror("minishell: heredoc:");
		return (0);
	}
	status = capture_heredoc(msh, delim, heredoc_fd[1]);
	close(heredoc_fd[1]);
	if (!status)
	{
		close(heredoc_fd[0]);
		return (-1);
	}
	return (heredoc_fd[0]);
}
