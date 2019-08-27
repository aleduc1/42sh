/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_set_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 14:54:24 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/27 08:17:34 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int			verif_syntax_key(char *key)
{
	while (*key)
	{
		if (ft_isalnum(*key) == 0 && (*key) != '_')
			return (0);
		++key;
	}
	return (1);
}

static void	apply_edit_set_no_command(char *key, char **value, int i, int cnt)
{
	reset_hash_verif(key);
	edit_setenv(key, value[i] + cnt + 1);
	// add_set_value(key, value[i] + cnt + 1);
}

void		apply_edit_set_no_command_env(char *key, char **value, int i,
		int cnt)
{
	reset_hash_verif(key);
	edit_setenv(key, value[i] + cnt + 1);
}

int			edit_set_no_command(char **value)
{
	char	*key;
	int		i;
	int		cnt;

	i = -1;
	while (value[++i])
	{
		if ((cnt = ft_chr_index(value[i], '=')) > 0)
		{
			key = ft_strsub(value[i], 0, cnt);
			if (verif_syntax_key(key) == 0)
			{
				ft_strdel(&key);
				return (-1);
			}
			else if (key && ft_strlen(key) > 0)
			{
				apply_edit_set_no_command(key, value, i, cnt);
				ft_strdel(&key);
			}
		}
		else
			break ;
	}
	return (i);
}
