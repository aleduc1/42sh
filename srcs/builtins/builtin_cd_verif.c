/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_verif.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 16:38:47 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/09 05:12:58 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int			ft_lastchr(const char *s, int c)
{
	int	cnt;
	int	last;

	last = -1;
	cnt = -1;
	while (s[++cnt])
		if (s[cnt] == c)
			last = cnt;
	if (last != -1)
		return (last);
	return (cnt);
}

void		check_arg(char **str)
{
	if (ft_strequ(*str, "-"))
	{
		ft_strdel(&(*str));
		(*str) = value_line_path("OLDPWD", 0);
	}
	else if (ft_strequ(*str, "--"))
	{
		ft_strdel(&(*str));
		(*str) = value_line_path("HOME", 0);
	}
}

void		error_cd(int code, char *str)
{
	struct stat	st;

	if (code == 1)
	{
		ft_dprintf(2, "21sh: cd: Too many args for cd command\n");
		return ;
	}
	code = access(str, F_OK);
	if (code == -1)
	{
		ft_dprintf(2, "21sh: cd: The directory '%s'", str);
		ft_dprintf(2, " does not exist\n");
		return ;
	}
	stat(str, &st);
	if (!S_ISDIR(st.st_mode))
	{
		ft_dprintf(2, "21sh: cd: '%s' is not a directory\n", str);
		return ;
	}
	code = access(str, X_OK);
	if (code == -1)
		ft_dprintf(2, "21sh: cd: Permission denied: '%s'\n", str);
	else
		ft_dprintf(2, "21sh: cd: An unknown error has occurred.\n");
}
