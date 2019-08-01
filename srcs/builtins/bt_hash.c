/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/14 14:52:42 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/25 17:33:59 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern int			g_optind;
extern t_ht_hash	*g_hash_table;

static void			bt_hash_showall(void)
{
	ht_hash_show(g_hash_table, 0);
}

static int			bt_addhash(char *av, int ret)
{
	char	*path;

	path = NULL;
	path = is_in_path(av);
	if (path != NULL)
	{
		ht_hash_insert(g_hash_table, av, path);
		ft_strdel(&path);
	}
	else
		ret = 1;
	return (ret);
}

static int			hash_getopt(char **av, int *r)
{
	int		i;
	char	arg;
	int		ac;

	ac = 0;
	while (av[ac])
		++ac;
	while ((arg = ft_getopt(ac, av, "r")) != -1)
	{
		if (arg == 'r')
			*r = 1;
		else
		{
			ft_dprintf(2, "hash: -%c: invalid option\n");
			ft_dprintf(2, "usage: hash [-r] [name...]\n");
			ft_getopt_reset();
			return (0);
		}
	}
	i = g_optind;
	ft_getopt_reset();
	return (i);
}

int					bt_hash(char **av)
{
	int	i;
	int	ret;
	int	r;

	r = 0;
	if ((i = hash_getopt(av, &r)) < 1)
		return (0);
	if (r == 1)
		ht_hash_reset(&g_hash_table);
	if (av[1] == NULL)
	{
		bt_hash_showall();
		return (0);
	}
	i = 1;
	ret = 0;
	while (av[i] != NULL)
	{
		if (!ft_strchr_exist(av[i], '/'))
			ret = bt_addhash(av[i], ret);
		++i;
	}
	return (ret);
}
