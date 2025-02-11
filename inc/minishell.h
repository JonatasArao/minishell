/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:36:34 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/11 09:54:49 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_command
{
	t_list	*arguments;
	t_list	*input_redir;
	t_list	*output_redir;
	int		exit_status;
}	t_command;

typedef struct s_redirection
{
	char	*type;
	char	*target;
}	t_redirection;

typedef struct s_minish
{
	char	*input;
	t_list	*tokens;
	t_list	*commands;
}	t_minish;

size_t		get_token_end(const char *s, unsigned int start);

char		*get_next_token(const char *s);

t_list		*extract_tokens(const char *s);

int			is_valid_quotes(const char *s);

int			is_input_redirection(const char *token);

int			is_output_redirection(const char *token);

int			is_redirection(const char *token);

int			is_pipe(const char *token);

int			is_token_list_valid(t_list *tokens);

void		free_redirection(void *redir_ptr);

void		free_command(void *cmd_ptr);

int			lstadd_str(t_list **head, char *str);

int			lstadd_redir(t_list **head, char *type, char *target);

t_command	*alloc_command(void);

t_list		*parse_tokens(t_list *token_node, t_command *cmd);

t_list		*extract_commands(t_list *tokens);

#endif
