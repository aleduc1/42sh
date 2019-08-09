/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:04:50 by aleduc            #+#    #+#             */
/*   Updated: 2019/08/08 21:54:31 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Faire :
		-b pathname : true is pathname block special file
		-c pathname : true if pathname character special file
		-d pathname : true if pathname directory
		-e pathname : true if pathname exists
		-f pathname : true if pathname regular file
		-g pathname : true if pathname file with set-group-ID flag set
		-L pathname : true if pathname symbolic link
		-p pathname : true if pathname FIFO
		-r pathname : true if pathname file with read perm
		-S pathname : true if pathname socket
		-s pathname : true if pathname file size greater than 0
		-u pathname : true if pathname file with set-user-ID flag set
		-w pathname : true if pathname file with write perm
		-x pathname : true if pathname file with exec perm
		-z string : true if string lenght is 0
		string : true if string not null
		s1 = s2 : true if s1 and s2 identical
		s1 != s2 : true if s1 and s2 not identical
		n1 -eq n2 : true if int n1 and n2 equal
		n1 -ne n2 : true if int n1 and n2 not equal
		n1 -gt n2 : true if int n1 greater than n2
		n1 -ge n2 : true if int n1 greater or equal to n2
		n1 -lt n2 : true if int n1 less than n2
		n1 -le n2 : true if int n1 less or equal to n2 
		! expression : true if expression false
*/

int		test_unop(char **av, int ind)
{
	if ((ft_strequ(av[ind], "-b") || (ft_strequ(av[ind], "-c"))))
		return (1);
	else if ((ft_strequ(av[ind], "-d") || (ft_strequ(av[ind], "-e"))))
		return (1);
	else if ((ft_strequ(av[ind], "-f") || (ft_strequ(av[ind], "-g"))))
		return (1);
	else if ((ft_strequ(av[ind], "-L") || (ft_strequ(av[ind], "-p"))))
		return (1);
	else if ((ft_strequ(av[ind], "-r") || (ft_strequ(av[ind], "-S"))))
		return (1);
	else if ((ft_strequ(av[ind], "-s") || (ft_strequ(av[ind], "-u"))))
		return (1);
	else if ((ft_strequ(av[ind], "-w") || (ft_strequ(av[ind], "-x"))))
		return (1);
	else if ((ft_strequ(av[ind], "-z"))
		return (1);
	return (0);
}

int		test_binop(char **av, int ind)
{
	if ((ft_strequ(av[ind], "-eq") || (ft_strequ(av[ind], "-ne"))))
		return (1);
	else if ((ft_strequ(av[ind], "-gt") || (ft_strequ(av[ind], "-ge"))))
		return (1);
	else if ((ft_strequ(av[ind], "=") || (ft_strequ(av[ind], "!="))))
		return (1);
	else if ((ft_strequ(av[ind], "-lt") || (ft_strequ(av[ind], "-le"))))
		return (1);
	return (0);
}

int		test_rev(int i)
{
	if (i == 1)
		return (0);
	else if (i == 0)
		return (1);
	return (i);
}

int		test_one(char **av, int ind)
{
	if (av[ind][0] == '\0')
		return (1);
	return (0);
}

int		test_two(char **av, int ind)
{
	if (ft_strequ(av[ind], "!"))
		return ((test_rev(test_one(av, 2))));
}

int		test_three(char **av, int ind)
{

}

int		test_four(char **av, int ind)
{

}

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
	int	ac;

	ac = 1;
	while (av[i])
		++i;
	if (ac == 1)
		return (1);
	else if (ac == 2)
		return (test_one(av, 1));
	else if (ac == 3)
		return (test_two(av, 1));
	else if (ac == 4)
		return (test_three(av, 1));
	else if (ac == 5)
		return (test_four(av, 1));
	else
	{
		ft_dprintf(r->error, "test: too many arguments\n");
		return (1);
	}
}
