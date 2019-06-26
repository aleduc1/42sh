/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_type.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 16:09:25 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/25 17:59:17 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;
extern t_ht_hash	*g_hash_table;

static int			type_alias_hash(char *command)
{
	char	*val;

	if ((val = ht_hash_search(g_alias_table, command)))
	{
		ft_printf("%s is aliased to `%s'\n", command, val);
		return (1);
	}
	else if ((val = ht_hash_search(g_hash_table, command)))
	{
		ft_printf("%s is hashed (%s)\n", command, val);
		return (1);
	}
	return (0);
}

int					bt_type(char **av)
{
	int		i;
	int		ret;
	char	*arg;

	i = 1;
	ret = 0;
	while (av[i])
	{
		if (type_alias_hash(av[i]));
		else if ((arg = is_in_path(av[i])))
		{
			ft_printf("%s is %s\n", av[i], arg);
			ft_strdel(&arg);
		}
		else
		{
			ft_dprintf(2, "type: %s: not found\n", av[i]);
			ret = 1;
		}
		++i;
	}
	return (ret);
}
