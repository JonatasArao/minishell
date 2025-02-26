/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:36:34 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/26 11:27:53 by jarao-de         ###   ########.fr       */
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
	t_list	*redirections;
	pid_t	pid;
	int		input_fd;
	int		output_fd;
	int		status;
}	t_command;

typedef struct s_redirection
{
	char	*type;
	char	*target;
}	t_redirection;

typedef struct s_env_var
{
	char	*key;
	char	*value;
}	t_env_var;

typedef struct s_minish
{
	char	*input;
	t_list	*tokens;
	t_list	*commands;
	t_list	*env_vars;
	int		last_status;
}	t_minish;

size_t		get_token_end(const char *s, unsigned int start);

char		*get_next_token(const char *s);

t_list		*extract_tokens(const char *s);

int			is_valid_quotes(const char *s);

int			is_heredoc(const char *token);

int			is_input_redirection(const char *token);

int			is_output_redirection(const char *token);

int			is_redirection(const char *token);

int			is_pipe(const char *token);

int			is_token_list_valid(t_list *tokens);

void		free_redirection(void *redir_ptr);

void		free_command(void *cmd_ptr);

int			lstadd_str(t_list **head, const char *str);

int			lstadd_redir(t_list **head, const char *type, const char *target);

t_command	*alloc_command(void);

t_list		*parse_tokens(t_list *token_node, t_command *cmd);

t_list		*extract_commands(t_list *tokens);

void		free_env_var(void *env_var_ptr);

int			lstadd_env_var(t_list **env, const char *key, const char *value);

t_env_var	*get_env_var(t_list *env, const char *key);

int			lstset_env_var(t_list **env, const char *key, const char *value);

int			lstrm_env_var(t_list **env, const char *key);

t_list		*extract_env_vars(char **envp);

char		*create_env_string(const char *key, const char *value);

char		**get_envp(t_list *env);

size_t		get_var_end(const char *token);

char		*get_next_var(char const *token);

t_list		*extract_vars(const char *token);

char		*get_var_value(t_list *env, int last_status, const char *key);

char		*concat_vars(t_list *vars);

char		*expand_quotes(char *s);

int			expand_var(t_list *env, int last_status, char **var);

char		*expand_token(t_list *env, int last_status, char *token);

int			is_ambiguous_redirect(char *old_target, char *new_target);

int			expand_redir(t_list *env, int last_status, t_list *redir);

int			expand_arguments(t_list *env, int last_status, t_list **arguments);

t_list		*expand_commands(t_list *env, int last_status, t_list *cmds);

void		free_minishell_loop(t_minish *msh);

void		init_minishell(t_minish *msh, char **envp);

void		destroy_minishell(t_minish *msh);

int			process_input(t_minish *msh);

int			msh_echo(t_command *cmd);

int			msh_pwd(t_command *cmd);

int			msh_cd(t_list **env, t_command *cmd);

int			msh_env(t_list *env, t_command *cmd);

int			msh_export(t_list **env, t_command *cmd);

int			msh_unset(t_list **env, t_command *cmd);

int			msh_exit(t_minish *msh, t_command *cmd);

int			launch_builtin(t_minish *msh, t_command *cmd);

#endif
