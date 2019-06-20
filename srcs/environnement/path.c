/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/02 18:24:02 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
	char	*result;

	result = NULL;
	if (!command)
		return (NULL);
	result = check_env_path(command);
	if (result)
		return (result);
	if (access(command, F_OK) >= 0)
	{
		result = ft_strdup(command);
		check_exec_path(&result);
		return (result);
	}
	return (result);
}
