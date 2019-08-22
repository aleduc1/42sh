/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 14:54:24 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/22 17:48:41 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"

int			bt_export(char **value, t_redirection *r)
{
	int		i;
	int		verif;
	char	**spl;

	i = -1;
	verif = 0;
	if (!value[1])
	{
		builtin_env_display(r);
		return (0);
	}
	while (value[++i])
	{
		if (ft_strchr_exist(value[i], '='))
		{
			spl = ft_strsplit(value[i], '=');
			if (spl && spl[0])
			{
				if (verif_syntax_key(spl[0]))
					verif = (edit_setenv(spl[0], spl[1]) == -1) ? 1 : 0;
				else
					verif = 1;
			}
			ft_arraydel(&spl);
		}
		else
			verif = (edit_export(value[i]) == 1) ? 0 : 1;
	}
	return (verif);
}
