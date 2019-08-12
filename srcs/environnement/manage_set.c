/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 14:54:24 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:18:42 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

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
			add_set_value(key, value[i] + cnt + 1);
			ft_strdel(&key);
		}
		else
			break ;
	}
	return (i);
}

int			edit_set_command(char **value, t_redirection *r, t_pos *pos)
{
	t_env	*cpy_env;
	int		result;

	cpy_env = ft_cpy_env();
	result = edit_set_no_command(value);
	ft_simple_command_redirection(value + result, r, pos, 1);
	get_env(1, NULL);
	get_env(0, cpy_env);
	return (result);
}

int			edit_set(char **value, t_redirection *r, t_pos *pos)
{
	int		result;
	int		i;

	i = -1;
	result = 0;
	while (value[++i])
		if (ft_chr_index(value[i], '=') < 2)
			break ;
	if (!value[i])
		result = edit_set_no_command(value);
	else
		edit_set_command(value, r, pos);
	return (result);
}

int			ft_unset(char **value)
{
	int	verif;
	int	i;

	i = 0;
	while (value[++i])
		verif = free_maillon_env(value[i], 0);
	return (verif);
}

int			edit_set_command_env(char *str, t_env *my_env)
{
	int		verif;
	char	**spl;

	verif = 0;
	spl = ft_strsplit(str, '=');
	if (!spl)
		return (-1);
	while (my_env && my_env->next)
	{
		if (ft_strequ(my_env->key, spl[0]))
		{
			ft_strdel(&((my_env)->value));
			my_env->value = ft_strdup(spl[1] ? spl[1] : "");
			verif = 1;
			break ;
		}
		my_env = my_env->next;
	}
	if (verif == 0)
		verif = create_new_line_env(my_env, spl[0], spl[1], 0);
	if (verif != -1)
		verif = edit_export(spl[0]);
	ft_arraydel(&spl);
	return (verif);
}
