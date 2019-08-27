/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_set_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 14:54:24 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/27 05:50:03 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int			add_set_value_perm(char *key, char *value, int perm)
{
	t_env	*my_env;
	int		verif;

	verif = 0;
	reset_hash_verif(key);
	my_env = get_env(0, NULL);
	if ((!key) || ft_strlen(key) < 1)
		return (-1);
	if (!value)
		value = "";
	while (my_env->next)
	{
		if (ft_strequ(my_env->key, key) && my_env->see_env == perm)
		{
			ft_strdel(&((my_env)->value));
			my_env->value = ft_strdup(value ? value : "");
			verif = 1;
			break ;
		}
		else if (ft_strequ(my_env->key, key) && my_env->see_env != perm)
			return (0);
		my_env = my_env->next;
	}
	if (verif == 0)
		verif = create_new_line_env(my_env, key, value, perm);
	return (verif);
}

int			add_set_value(char *key, char *value)
{
	return (add_set_value_perm(key, value, 0));
}
