/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:04:50 by aleduc            #+#    #+#             */
/*   Updated: 2019/07/01 18:57:39 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		bt_test(char **av, t_redirection *r)
{
/*
** Divide by arguments found, for each argument call a function
** In each functions apply rules for each arguments
** Some function will call other depending (if 4 args and arg[0] = ! then call arg3 funct and negate the return value of arg3)
** For all unspecified case like when arg > 4 print a message on stderr and return 1
** In every function of argument apply precedence rule with first ! then checking if binary primaries needed, else apply unary primarie
** Regroup the 15 unary operator in a funct -> is_unary and do the same for is_binary
** When precedence rules are done and I need to evaluate the unarie primaries or binary primaries expression call the adequat funct with a tab of funct pointer
** Then in each func apply each rule needed depending on the "-primary operator"
** Each positive return is 0. Every false return is 1. Whether it is for small unary primaries expressions, or binary primaries, or arg[number] func, or bt_test.
*/
}
