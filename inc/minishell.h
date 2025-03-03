/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:36:34 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/01 20:56:57 by jarao-de         ###   ########.fr       */
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

typedef struct s_minish
{
	char	*input;
	t_list	*tokens;
	t_list	*commands;
	t_list	*env_vars;
	int		saved_fd[2];
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

int			is_builtin(t_command *cmd);

int			launch_builtin(t_minish *msh, t_command *cmd);

char		**get_argv(t_list *args);

int			backup_fds(t_minish *msh);

void		restore_fds(t_minish *msh);

int			apply_input_redirection(int input_fd);

int			apply_output_redirection(int output_fd);

int			apply_redirections(t_command *cmd);

int			wait_pipeline(pid_t last_pid, int num_commands);

int			execute_pipeline(t_minish *msh, int num_commands);

int			wait_single(pid_t pid);

int			execute_single(t_minish *msh);

int			execute_commands(t_minish *msh);

char		*get_full_path(char *token, char *cmd);

char		*search_path(char *path, char *cmd);

char		*get_command_path(t_list *env, char *cmd);

int			launch_process(t_minish *msh, t_list *cmd_node,
				int (*launcher)(t_minish *, t_command *));

void		child_process(t_minish *msh, t_list *cmd_node, int input_fd,
				int (*launcher)(t_minish *, t_command *));

int			parent_process(t_list *cmd_node, pid_t pid, int input_fd);

void		free_execution(char *path, char **argv, char **envp);

int			is_directory(char *path);

int			get_exec_error(char *path, char **argv);

int			launch_executable(t_minish *msh, t_command *cmd);

int			is_fd_open(int fd, char *target);

int			open_output_redirections(t_redirection *redir, int *output_fd);

int			open_input_redirections(t_command *cmd, t_redirection *redir,
				int *input_fd);

int			open_redirections(t_command *cmd);

size_t		get_heredoc_var_end(const char *token);

char		*get_next_heredoc_var(char const *token);

t_list		*extract_heredoc_vars(const char *token);

int			expand_heredoc_var(t_list *env, int last_status, char **var);

char		*expand_heredoc(t_list *env, int last_status, char *line);

int			remove_delim_quotes(char *str);

int			is_delimiter(char *input, char *delim);

char		*process_input_line(t_minish *msh, char *input, int has_quote);

int			capture_heredoc(t_minish *msh, char *delim, int heredoc_fd);

int			open_heredoc(t_minish *msh, char *delim);

int			apply_heredoc(t_minish *msh, t_command *cmd,
				t_redirection *redir);

int			setup_heredocs(t_minish *msh);

#endif
