/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_unalias.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 17:00:29 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/14 15:27:25 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;

int		bt_unalias(char **av)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!av[1])
	{
		ft_dprintf(2, "unalias: not enough arguments\n");
		return (1);
	}
	while (av[i])
	{
		if (ht_hash_search(g_alias_table, av[i]) != NULL)
			ht_hash_delete(g_alias_table, av[i]);
		else
		{
			ft_dprintf(2, "unalias: no such hash table element: %s\n", av[i]);
			ret = 1;
		}
		++i;
	}
	return (ret);
}
