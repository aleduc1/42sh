/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 17:22:06 by apruvost          #+#    #+#             */
/*   Updated: 2019/05/08 03:28:00 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		cd_getnextpath(char *path)
{
	int	i;

	i = 0;
	while (path[i] != '\0' && path[i] != ':')
		i++;
	if (path[i] != '\0')
		return (i + 1);
	return (i);
}

int		cd_testpath(char *path, char *directory, char **test)
{
	int		i;
	char	*tmp;

	i = ft_strlen(path);
	if (path[i - 1] == '/')
		tmp = ft_strjoin(path, directory);
	else
		tmp = ft_jointhree(path, "/", directory);
	if (access(tmp, F_OK) == 0)
	{
		*test = tmp;
		return (1);
	}
	ft_strdel(&tmp);
	return (0);
}

/*
** Function to test a pathname of CDPATH
*/

int		cd_testcdpath(char *path, char *directory, char **test)
{
	int		i;
	int		j;
	char	*tmp;

	i = cd_getnextpath(path);
	if ((tmp = (char *)malloc(sizeof(char) * (i + 1))) == NULL)
	{
		dprintf(2, "21sh: cd: Failed to malloc\n");			// NO MEMORY
		return (0);
	}
	j = cd_testpath(tmp, directory, test);
	ft_strdel(&tmp);
	return (j);
}
