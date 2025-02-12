/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:03:30 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/12 17:05:18 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arguments(t_list *arguments)
{
	t_list	*node;

	node = arguments;
	while (node)
	{
		printf("Argument: %s\n", (char *)node->content);
		node = node->next;
	}
}

void	print_redirections(t_list *redirections, const char *type)
{
	t_redirection	*redir;
	t_list			*node;

	node = redirections;
	while (node)
	{
		redir = (t_redirection *)node->content;
		printf("%s Redirection: %s %s\n", type, redir->type, redir->target);
		node = node->next;
	}
}

void	print_commands(t_list *commands)
{
	t_command	*cmd;

	while (commands)
	{
		cmd = (t_command *)commands->content;
		print_arguments(cmd->arguments);
		print_redirections(cmd->input_redir, "Input");
		print_redirections(cmd->output_redir, "Output");
		printf("----------\n");
		commands = commands->next;
	}
}

void	process_input(t_minish *msh)
{
	t_command	*cmd;
	t_env_var	*env_var;

	msh->tokens = extract_tokens(msh->input);
	if (msh->tokens && is_token_list_valid(msh->tokens))
	{
		msh->commands = extract_commands(msh->tokens);
		if (msh->commands)
		{
			print_commands(msh->commands);
			cmd = (t_command *)msh->commands->content;
			env_var = get_env_var(msh->env_vars,
					(char *) cmd->arguments->content);
			if (env_var && env_var->value)
				printf("Env:%s\nValue:%s\n", env_var->key, env_var->value);
			ft_lstclear(&msh->commands, free_command);
		}
	}
	if (msh->tokens)
		ft_lstclear(&msh->tokens, free);
}

int	main(int argc, char **argv, char **envp)
{
	t_minish	msh;

	(void) argc;
	(void) argv;
	msh.env_vars = extract_env_vars(envp);
	if (!msh.env_vars)
		return (1);
	while (1)
	{
		msh.input = readline("$ ");
		if (msh.input == NULL)
			break ;
		if (ft_strncmp(msh.input, "exit", 4) == 0 && msh.input[4] == '\0')
		{
			free(msh.input);
			break ;
		}
		process_input(&msh);
		if (!ft_strall(msh.input, ft_isspace))
			add_history(msh.input);
		free(msh.input);
	}
	ft_lstclear(&msh.env_vars, free_env_var);
	rl_clear_history();
	return (0);
}
