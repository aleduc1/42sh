/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_test_unopone.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:11:48 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/13 21:28:04 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		test_unop_exist(char *dir)
{
	if (!(access(dir, F_OK)))
		return (0);
	return (1);
}

int		test_unop_read(char *dir)
{
	if (!(access(dir, R_OK)))
		return (0);
	return (1);
}

int		test_unop_write(char *dir)
{
	if (!(access(dir, W_OK)))
		return (0);
	return (1);
}

int		test_unop_exec(char *dir)
{
	if (!(access(dir, X_OK)))
		return (0);
	return (1);
}

int		test_unop(char **av, int ind)
{
	if (ft_strequ(av[ind], "-b"))
		return (test_unop_block(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-c"))
		return (test_unop_charac(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-d"))
		return (test_unop_direct(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-e"))
		return (test_unop_exist(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-f"))
		return (test_unop_file(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-g"))
		return (test_unop_group(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-L"))
		return (test_unop_slink(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-p"))
		return (test_unop_fifo(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-r"))
		return (test_unop_read(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-S"))
		return (test_unop_socket(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-s"))
		return (test_unop_size(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-u"))
		return (test_unop_user(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-w"))
		return (test_unop_write(av[++ind]));		//		X
	else if (ft_strequ(av[ind], "-x"))
		return (test_unop_exec(av[++ind]));			//		X
	else if (ft_strequ(av[ind], "-z"))
		return (test_unop_zero(av[++ind]));			//		X
	return (1);
}
