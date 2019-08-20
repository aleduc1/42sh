/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:04:50 by aleduc            #+#    #+#             */
/*   Updated: 2019/08/20 03:25:38 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Regroup the 15 unary operator in a funct -> is_unary and do the same for is_binary
** When precedence rules are done and I need to evaluate the unarie primaries or binary primaries expression call the adequat funct with a tab of funct pointer
** Then in each func apply each rule needed depending on the "-primary operator"
** Each positive return is 0. Every false return is 1. Whether it is for small unary primaries expressions, or binary primaries, or arg[number] func, or bt_test.
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
		return (bt_5(void));
}
