/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:04:50 by aleduc            #+#    #+#             */
/*   Updated: 2019/08/22 07:49:58 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		unary_test(char *str1, char *str2)
{
	int			i;
	t_unary_tab	*u_tab;

	i = 0;
	u_tab = NULL;
	create_unary_list(&u_tab);
	while (u_tab[i].f)
	{
		if (ft_strcmp(str1, u_tab[i].str) == 0)
			return (u_tab[i].f(str1, str2, str1));
		i++;
	}
	return (1);
}

int		binary_test(char *str1, char *str2, char *str3)
{
	int			i;
	t_unary_tab	*b_tab;

	i = 0;
	b_tab = NULL;
	create_binary_list(&b_tab);
	while (b_tab[i].f)
	{
		if (ft_strcmp(str2, b_tab[i].str) == 0)
			return (b_tab[i].f(str1, str2, str3));
		i++;
	}
	ft_putendl("IM not supposed to be here BOIIIII");
	return (1);
}

int		is_binary(char *str2)
{
	if (ft_strcmp(str2, "=") == 0)
		return (1);
	if (ft_strcmp(str2, "!=") == 0)
		return (1);
	if (ft_strcmp(str2, "-eq") == 0)
		return (1);
	if (ft_strcmp(str2, "-ne") == 0)
		return (1);
	if (ft_strcmp(str2, "-ge") == 0)
		return (1);
	if (ft_strcmp(str2, "-lt") == 0)
		return (1);
	if (ft_strcmp(str2, "-le") == 0)
		return (1);
	if (ft_strcmp(str2, "=") == 0)
		return (1);
	return (0);
}

/*
** Pense a mettre des commentaires et LIS les warnings:
** #define tab  CUR Strings[134] > tab est une macro utilise par la bibliotheque
** term.h tu avais juste a renommer ta variable
*/

int		bt_test(char **av, t_redirection *r)
{
	int	ac;

	ac = 0;
	redirection_fd(r);
	while (av[ac])
		ac++;
	if (ac == 0)
		return (1);
	if (ac == 1)
		return (bt_1(av[1]));
	if (ac == 2)
		return (bt_2(av[1], av[2]));
	if (ac == 3)
		return (bt_3(av[1], av[2], av[3]));
	if (ac == 4)
		return (bt_4(av[1], av[2], av[3], av[4]));
	if (ac > 4)
		return (bt_5());
	return (0);
}
