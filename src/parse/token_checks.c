/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 00:33:33 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/10 00:36:45 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_input_redirection(const char *token)
{
	if ((ft_strncmp(token, "<<", 2) == 0 && token[2] == '\0')
		|| (token[0] == '<' && token[1] == '\0'))
		return (1);
	return (0);
}

int	is_output_redirection(const char *token)
{
	if ((ft_strncmp(token, ">>", 2) == 0 && token[2] == '\0')
		|| (token[0] == '>' && token[1] == '\0'))
		return (1);
	return (0);
}

int	is_redirection(const char *token)
{
	if (is_input_redirection(token) || is_output_redirection(token))
		return (1);
	return (0);
}

int	is_pipe(const char *token)
{
	if (ft_strncmp(token, "|", 1) == 0 && token[1] == '\0')
		return (1);
	return (0);
}
