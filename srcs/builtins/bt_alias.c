/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 17:00:16 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/25 17:33:41 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;

static void			bt_alias_show(void)
{
	ht_hash_show(g_alias_table, 1);
}

static int			bt_showalias(char **av, int i, int ret)
{
	char	*val;

	val = ht_hash_search(g_alias_table, av[i]);
	if (val == NULL)
		return (1);
	ft_printf("%s=%s\n", av[i], val);
	return (ret);
}

static void				bt_addalias(char **av, int i)
{
	char	*tmp;

	tmp = ft_strchr(av[i], '=');
	*tmp = '\0';
	ht_hash_insert(g_alias_table, av[i], &(tmp[1]));
	*tmp = '=';
}

int					bt_alias(char **av)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!av[1])
		bt_alias_show();
	while (av[i])
	{
		if (ft_strchr_exist(av[i], '='))
			bt_addalias(av, i);
		else
			ret = bt_showalias(av, i, ret);
		++i;
	}
	return (ret);
}
