/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 17:56:32 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/11 16:34:26 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** cd [-L|-P] [directory]
** cd -
**
** cd utility steps (*curpath* is an intermediate value) :
**
** 1 - If no directory is given and HOME env empty or unset : implementation defined behavior and stop
**
** 2 - If no directory is given and HOME env set to no empty : directory is HOME env value
**
** 3 - If directory starts with '/' : set *curpath* to directory and go step 7
**
** 4 - If first component of directory is '.' or '..' : go step 6
**
** 5 - If ':' separated pathname of CDPATH set : test with each "pathname/directory"
**     If no pathname : test "./directory"
**     If result points to directory : set curpath to that string and go step 7
**
** 6 - Set curpath to directory
**
** 7 - If -P option : go step 10
**     If curpath does not begin with '/' : set curpath to "PWD/curpath"
**
** 8 - curpath shall be converted to canonical form as follows :
**         a. dot components and any '/' that separate them from the next component shall be deleted
**         b. For each '..', if there is a preceding component and it is neither root nor '..', then :
**             i. If preceding component does not refer directory (ex. symbolic link), display error message and stop
**             ii. Preceding component, '/', '..' and '/' (if any) shall be deleted
**         c. An implementation ma further simplify curpath b removing any trailng '/' that are not also leading '/',
**            replacing multiple consecutive '/' by a single '/', and replacing three or more leading '/' by a single '/'
**            If after step 8, curpath is null, stop
**
** 9 - If curpath longer than {PATH_MAX} (including ter null) and directory not longer than {PATH_MAX} (including ter null) :
**     curpath shall be converted from absolute pathname to equ relative pathname if possible
**         This conversion always considered possible if PWD, with trailing '/' is an initial substring of 
**         Whether or not considered possile under other circumstances unspecified
**     Implementations may also apply this conversio if curpath not longer than {PATH_MAX} or directory was longer than {PATH_MAX}
**
** 10 - cd shall use equ to chdir() function with curpath as path argument
**      If these action fail for any reason, display error and stop
**      If not -P option : PWD env var set to curpath before step 9
**      If -P option : PWD env var set to string that would be output by pwd -P (getcwdir)
*       If insufficient permission o new directory, or any parent directory, to determine current working directory, value PWD unspecified
**
** If, during above steps, PWD variable is set, change OLD_PWD to value of old working directory (current working directory prior to cd call) 
*/

int			is_env_empty(char *value)
{
	char *str;

	str = value_line_path(value, 0);
	if (str)
	{
		if (ft_strequ(str, ""))
		{
		ft_strdel(&str);
		return (1);
		}
		else
		{
		ft_strdel(&str);
		return (0);
		}
	}
	return (1);
}

static int	cd_stepten(t_cd *cd)					// Step 10
{
	struct stat	sb;

	if (stat(cd->curpath, &sb) == -1)
	{
		dprintf(2, "21sh: cd: permission denied\n");
		return (cd_err(cd));
	}
	if ((sb.st_mode & S_IFMT) != S_IFDIR)
	{
		dprintf(2, "21sh: cd: no such file or directory: %s\n", cd->directory);
		return (cd_err(cd));
	}
	if (cd_chdir(cd))
		return (cd_err(cd));
	return (0);
}

static int	cd_stepsev(t_cd *cd)
{
	char	*bin;
	char	*binn;

	if (cd->arg_P == 1)
		return (cd_stepten(cd));
	if (cd->curpath[0] != '/')
	{
		bin = value_line_path("PWD", 0);
		binn = cd->curpath;
		cd->curpath = ft_jointhree(bin, "/", cd->curpath);
		ft_strdel(&binn);
		ft_strdel(&bin);
	}
	if (!cd_canonical(cd))											// Step 8
		return (cd_err(cd));
//	cd_path_max(cd, env);										// Step 9
	return (cd_stepten(cd));
}

static int	cd_stepfive(t_cd *cd)
{
	char	*test;
	char	*path;
	int		j;

	test = NULL;
	if (is_env_empty("CDPATH"))
	{
		if (!cd_testpath(".", cd->directory, &test))
			return (0);
		cd->curpath = test;
		return (1);
	}
	path = value_line_path("CDPATH", 0);
	j = 7;
	while (path[j] != '\0')
	{
		if ((cd_testcdpath(&(path[j]), cd->directory, &test)))
		{
			cd->curpath = test;
			ft_strdel(&path);
			return (1);
		}
		j += cd_getnextpath(&(path[j]));
	}
	ft_strdel(&path);
	return (0);
}

static int	cd_init(char **av, t_cd *cd)
{
	int		i;
	int		ac;

	cd->arg__ = 0;
	cd->arg_L = 1;
	cd->arg_P = 0;
	cd->curpath = NULL;
	ac = 1;
	while (av[ac])
		ac++;
	if (!(i = cd_getopt(ac, av, cd)))
		return (0);
	if (!av[i])
	{
		cd->directory = NULL;
		return (1);
	}
	if (ft_strequ(av[i], "-"))
	{
		cd->directory = value_line_path("OLDPWD", 0);
		cd->arg__ = 1;
		return (1);
	}										// V
	cd->directory = ft_strdup(av[i]);
	return (1);
}


int			bt_cd(char **av)
{
	t_cd	cd;

	if (!cd_init(av, &cd))							// V
		return (1);
	if (cd.directory == NULL && is_env_empty("HOME"))	// Step 1
		return (cd_err(&cd));
	if (cd.directory == NULL)									// Step 2
		cd.directory = value_line_path("HOME", 0);
	if (cd.directory[0] == '/')									// Step 3
	{
		cd.curpath = ft_strdup(cd.directory);
		return (cd_stepsev(&cd));
	}
	if (cd.directory[0] != '.' ||
		(cd.directory[0] != '.' && cd.directory[1] != '.'))		// Step 4
	{
		if (cd_stepfive(&cd))								// Step 5
			return (cd_stepsev(&cd));
		dprintf(2, "21sh: cd: %s: No such file or directory\n", cd.directory);
		return (cd_err(&cd));
	}
	cd.curpath = ft_strdup(cd.directory);						// Step 6
	return (cd_stepsev(&cd));
}
