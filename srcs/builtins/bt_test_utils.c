/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:09:48 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/13 21:10:14 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		test_isunop(char **av, int ind)
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
	else if ((ft_strequ(av[ind], "-z")))
		return (1);
	return (0);
}

int		test_isbinop(char **av, int ind)
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
	if (!i || i == 1)
		return (!i);
	return (i);
}
