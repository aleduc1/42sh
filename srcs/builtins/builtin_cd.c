/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 21:20:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/15 11:17:07 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static int	check_return(char *arguments, int verif)
{
	char	*list;
	char	*stock;

	if (ft_strequ(arguments, ".."))
	{
		list = value_line_path("PWD", 0);
		if (list)
		{
			stock = ft_strsub(list, 0, ft_lastchr(list, '/'));
			ft_strdel(&list);
			if (stock)
			{
				verif = chdir(stock);
				ft_strdel(&stock);
				return (verif);
			}
		}
	}
	return (verif);
}

static int	launch_cd(char **str)
{
	char	buf[BUF_S];
	int		verif;

	ft_bzero(buf, BUF_S);
	if (!(*str))
		return (-1);
	check_arg(&(*str));
	if (!(*str))
		return (-1);
	verif = chdir(*str);
	if (verif < 0)
		verif = check_return(*str, verif);
	if (verif > -1)
	{
		ft_bzero(buf, BUF_S);
		getcwd(buf, BUF_S);
		add_set_value("PWD", buf);
	}
	else
		error_cd(verif, *str);
	return (verif);
}

int			builtin_cd(char **av)
{
	char	buf[BUF_S];
	int		len;
	int		verif;
	char	*str;

	len = ft_arraylen(av);
	if (len == 1 || (len == 2 && ft_strequ(av[1], "")))
		str = value_line_path("HOME", 0);
	else if (len > 2)
	{
		error_cd(1, NULL);
		return (-2);
	}
	else
		str = ft_strdup(av[1]);
	ft_bzero(buf, BUF_S);
	getcwd(buf, BUF_S - 1);
	verif = launch_cd(&str);
	add_set_value("OLDPWD", buf);
	if (str)
		ft_strdel(&str);
	return (verif);
}
