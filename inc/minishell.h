/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:36:34 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 03:08:51 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/types.h>
# include <fcntl.h>

typedef struct s_command
{
	t_list	*arguments;
	t_list	*redirections;
	int		pipe_fd[2];
	int		input_fd;
	int		output_fd;
	int		heredoc_fd;
	pid_t	pid;
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

typedef struct s_msh
{
	char	*input;
	t_list	*tokens;
	t_list	*commands;
	t_list	*env_vars;
	int		saved_fd[2];
	int		last_status;
}	t_msh;

t_msh		*get_minishell(void);

void		init_minishell(t_msh *msh, char **envp);

void		free_minishell_loop(t_msh *msh);

void		destroy_minishell(t_msh *msh);

void		sigint_action(int sig);

void		sigint_process_action(int sig);

void		sigint_heredoc_action(int sig);

int			is_valid_quotes(const char *s);

int			is_token_list_valid(t_list *tokens);

int			is_heredoc(const char *token);

int			is_input_redirection(const char *token);

int			is_output_redirection(const char *token);

int			is_redirection(const char *token);

int			is_pipe(const char *token);

t_list		*extract_tokens(const char *s);

t_list		*extract_commands(t_list *tokens);

int			lstadd_str(t_list **head, const char *str);

int			lstadd_redir(t_list **head, const char *type, const char *target);

void		free_redirection(void *redir_ptr);

void		free_command(void *cmd_ptr);

t_list		*extract_vars(const char *token);

char		*get_var_value(t_list *env, int last_status, const char *key);

char		*expand_token(t_list *env, int last_status, char *token);

t_list		*expand_commands(t_list *env, int last_status, t_list *cmds);

char		**get_envp(t_list *env);

t_list		*extract_env_vars(char **envp);

void		free_env_var(void *env_var_ptr);

t_env_var	*get_env_var(t_list *env, const char *key);

int			lstadd_env_var(t_list **env, const char *key, const char *value);

int			lstset_env_var(t_list **env, const char *key, const char *value);

int			lstrm_env_var(t_list **env, const char *key);

char		*concat_vars(t_list *vars);

int			open_redirections(t_command *cmd);

int			setup_heredocs(t_msh *msh);

int			launch_process(t_msh *msh, t_list *cmd_node,
				int (*launcher)(t_msh *, t_command *));

int			launch_executable(t_msh *msh, t_command *cmd);

int			is_builtin(t_command *cmd);

int			launch_builtin(t_msh *msh, t_command *cmd);

char		*get_command_path(t_list *env, char *cmd);

char		*expand_heredoc(t_list *env, int last_status, char *line);

int			execute_commands(t_msh *msh);

int			capture_heredoc(t_msh *msh, char *delim, int heredoc_fd);

char		**get_argv(t_list *args);

int			backup_fds(t_msh *msh);

void		restore_fds(t_msh *msh);

int			apply_redirections(t_command *cmd);

int			msh_unset(t_list **env, t_command *cmd);

int			msh_pwd(t_command *cmd);

int			msh_export(t_list **env, t_command *cmd);

int			msh_exit(t_msh *msh, t_command *cmd);

int			msh_env(t_list *env, t_command *cmd);

int			msh_echo(t_command *cmd);

int			msh_cd(t_list **env, t_command *cmd);

#endif
