/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 14:54:24 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 10:16:17 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"

int			bt_export(char **value)
{
	int		i;
	int		verif;
	char	**spl;

	i = -1;
	verif = 0;
	while (value[++i])
	{
		if (ft_strchr_exist(value[i], '='))
		{
			spl = ft_strsplit(value[i], '=');
			if (spl && spl[0])
				verif = edit_setenv(spl[0], spl[1]);
			ft_arraydel(&spl);
		}
		else
			verif = edit_export(value[i]);
	}
	return (verif);
}
