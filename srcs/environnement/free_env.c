/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 14:18:41 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/17 14:21:52 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static int	free_that(t_env *my_env, t_env *last)
{
	int	verif;

	verif = 0;
	if (last)
		last->next = my_env->next;
	else
	{
		last = my_env->next;
		verif = 1;
	}
	ft_strdel(&(my_env->key));
	ft_strdel(&(my_env->value));
	free(my_env);
	my_env = NULL;
	if (verif)
		get_env(0, last);
	return (1);
}

int			free_maillon_env(char *key, int env)
{
	int		verif;
	t_env	*my_env;
	t_env	*last;
	int		test;

	my_env = get_env(0, NULL);
	last = NULL;
	verif = 0;
	test = 0;
	while (my_env->next)
	{
		if (ft_strequ(my_env->key, key) && (env == 0 ||
					(env == 1 && my_env->see_env == 1)))
		{
			verif = free_that(my_env, last);
			break ;
		}
		last = my_env;
		my_env = my_env->next;
	}
	return (verif);
}

void		free_env(t_env **lst)
{
	if ((*lst)->next)
		free_env(&((*lst)->next));
	ft_strdel(&((*lst)->key));
	ft_strdel(&((*lst)->value));
	free(*lst);
	(*lst) = NULL;
}
