/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/20 01:04:40 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_hash_table;

static char	*get_hash(char *command)
{
	char	*str;

	str = ht_hash_search(g_hash_table, command);
	if (!str)
		return (NULL);
	return (ft_strdup(str));
}

/*
** split is all the path
*/

static void	check_exec_path(char **str)
{
	if (access(*str, X_OK) >= 0)
		return ;
	gest_return(-6);
	ft_strdel(str);
	return ;
}

static char	*path_of_commands(char *command, char **split)
{
	int		i;
	char	*str;
	char	*dst;

	i = -1;
	if (!split)
		return (NULL);
	while (split[++i])
	{
		dst = ft_strjoin(split[i], "/");
		str = ft_strjoin(dst, command);
		if (access(str, F_OK) >= 0)
		{
			ft_strdel(&dst);
			check_exec_path(&str);
			return (str);
		}
		ft_strdel(&str);
		ft_strdel(&dst);
	}
	return (NULL);
}

static char	*check_env_path(char *command)
{
	char	*result;
	char	*str;
	char	**split;

	str = value_line_path("PATH", 1);
	if (!str)
	{
		str = value_line_path("PATH", 0);
		if (!str)
			return (NULL);
	}
	split = ft_strsplit(str, ':');
	result = path_of_commands(command, split);
	ft_strdel(&str);
	ft_arraydel(&split);
	return (result);
}

char		*is_in_path(char *command)
{
	struct stat	statbuf;
	char		*result;

	result = NULL;
	if (!command)
		return (NULL);
	if ((result = get_hash(command)))
		return (result);
	result = check_env_path(command);
	if (result)
	{
		ht_hash_insert(g_hash_table, command, result);
		return (result);
	}
	if (stat(command, &statbuf) != 0)
		return (NULL);
	if (S_ISDIR(statbuf.st_mode) == 1)
		return (NULL);
	if (access(command, F_OK) >= 0 && command[0]
		&& command[1] && (command[0] == '/'
		|| (command[0] == '.' && command[1] == '/')))
	{
		result = ft_strdup(command);
		check_exec_path(&result);
		return (result);
	}
	return (result);
}
