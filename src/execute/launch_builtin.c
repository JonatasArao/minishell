/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:25:14 by jarao-de          #+#    #+#             */
/*   Updated: 2025/03/05 00:52:54 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_command *cmd)
{
	char	*cmd_name;

	if (!cmd->arguments || !cmd->arguments->content)
		return (0);
	cmd_name = (char *)cmd->arguments->content;
	if ((ft_strncmp(cmd_name, "echo", 4) == 0 && cmd_name[4] == '\0')
		|| (ft_strncmp(cmd_name, "pwd", 3) == 0 && cmd_name[3] == '\0')
		|| (ft_strncmp(cmd_name, "cd", 2) == 0 && cmd_name[2] == '\0')
		|| (ft_strncmp(cmd_name, "env", 3) == 0 && cmd_name[3] == '\0')
		|| (ft_strncmp(cmd_name, "export", 6) == 0 && cmd_name[6] == '\0')
		|| (ft_strncmp(cmd_name, "unset", 5) == 0 && cmd_name[5] == '\0')
		|| (ft_strncmp(cmd_name, "exit", 4) == 0 && cmd_name[4] == '\0'))
		return (1);
	return (0);
}

int	launch_builtin(t_msh *msh, t_command *cmd)
{
	char	*cmd_name;
	int		bi_return;

	bi_return = -1;
	cmd_name = (char *)cmd->arguments->content;
	if (ft_strncmp(cmd_name, "echo", 4) == 0 && cmd_name[4] == '\0')
		bi_return = msh_echo(cmd);
	else if (ft_strncmp(cmd_name, "pwd", 3) == 0 && cmd_name[3] == '\0')
		bi_return = msh_pwd(cmd);
	else if (ft_strncmp(cmd_name, "cd", 2) == 0 && cmd_name[2] == '\0')
		bi_return = msh_cd(&msh->env_vars, cmd);
	else if (ft_strncmp(cmd_name, "env", 3) == 0 && cmd_name[3] == '\0')
		bi_return = msh_env(msh->env_vars, cmd);
	else if (ft_strncmp(cmd_name, "export", 6) == 0 && cmd_name[6] == '\0')
		bi_return = msh_export(&msh->env_vars, cmd);
	else if (ft_strncmp(cmd_name, "unset", 5) == 0 && cmd_name[5] == '\0')
		bi_return = msh_unset(&msh->env_vars, cmd);
	else if (ft_strncmp(cmd_name, "exit", 4) == 0 && cmd_name[4] == '\0')
		bi_return = msh_exit(msh, cmd);
	return (bi_return);
}
