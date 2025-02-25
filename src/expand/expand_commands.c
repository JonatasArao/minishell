/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 23:19:19 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 15:18:45 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ambiguous_redirect(char *old_target, char *new_target)
{
	if (*old_target != '$'
		|| (new_target && *new_target && !ft_strchr(new_target, ' ')))
		return (0);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(old_target, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
	return (1);
}

int	expand_redir(t_list *env, int last_status, t_list *redir)
{
	t_list			*current;
	t_redirection	*current_redir;
	char			*old_target;
	char			*new_target;

	current = redir;
	while (current)
	{
		current_redir = (t_redirection *)current->content;
		if (!is_heredoc(current_redir->type))
		{
			old_target = current_redir->target;
			new_target = expand_token(env, last_status, old_target);
			if (is_ambiguous_redirect(old_target, new_target))
				ft_delpointer((void **) &new_target);
			if (!new_target)
				return (0);
			current_redir->target = new_target;
			free(old_target);
		}
		current = current->next;
	}
	return (1);
}

int	expand_arguments(t_list *env, int last_status, t_list **arguments)
{
	t_list	*current;
	char	*expanded_content;
	char	*old_content;

	current = *arguments;
	while (current)
	{
		old_content = (char *)current->content;
		expanded_content = expand_token(env, last_status, old_content);
		if (!expanded_content)
		{
			current = current->next;
			if (!ft_lstrm(arguments, old_content, free))
				return (0);
		}
		else
		{
			current->content = expanded_content;
			free(old_content);
			current = current->next;
		}
	}
	if (!(*arguments))
		return (0);
	return (1);
}

t_list	*expand_commands(t_list *env, int last_status, t_list *cmds)
{
	t_list		*current;
	t_command	*current_cmd;

	current = cmds;
	while (current)
	{
		current_cmd = (t_command *)current->content;
		if (!expand_redir(env, last_status, current_cmd->redirections)
			|| !expand_arguments(env, last_status, &current_cmd->arguments))
		{
			ft_lstclear(&cmds, free_command);
			return (NULL);
		}
		current = current->next;
	}
	return (cmds);
}
