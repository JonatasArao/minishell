/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_generator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:37:31 by jarao-de          #+#    #+#             */
/*   Updated: 2025/02/25 15:50:12 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_argv(t_list *args)
{
	t_list		*current;
	char		**argv;
	char		*arg;
	int			len;
	int			i;

	len = ft_lstsize(args);
	if (!len)
		return (NULL);
	argv = (char **) malloc((len + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	current = args;
	i = 0;
	while (current)
	{
		arg = (char *) args->content;
		argv[i] = ft_strdup(arg);
		if (!argv[i])
			return (ft_free_matrix((void **) argv, i));
		current = current->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
