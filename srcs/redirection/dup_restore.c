/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:22:31 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	stock_fd_base(void)
{
	// int	std[3];
	// char	*s_in;
	// char	*s_out;
	// char	*s_error;

	// std[0] = dup(STDIN_FILENO);
	// std[1] = dup(STDOUT_FILENO);
	// std[2] = dup(STDERR_FILENO);
	// s_in = ft_itoa(std[0]);
	// s_out = ft_itoa(std[1]);
	// s_error = ft_itoa(std[2]);
	// // if (s_in)
	// // 	add_set_value("STDIN", s_in);
	// // if (s_out)
	// // 	add_set_value("STDOUT", s_out);
	// // if (s_error)
	// // 	add_set_value("STDERR", s_error);
	// ft_strdel(&s_in);
	// ft_strdel(&s_out);
	// ft_strdel(&s_error);
}

void	dup_restore(void)
{
	// int		std[3];
	// char	*s_in;
	// char	*s_out;
	// char	*s_error;

	// s_in = value_line_path("in", 0);
	// s_out = value_line_path("out", 0);
	// s_error = value_line_path("error", 0);
	// if (s_in)
	// {
	// 	std[0] = ft_atoi(s_in);
	// 	ft_printf("la\n");
	// 	dup2(std[0], STDIN_FILENO);
	// 	close(std[0]);
	// 	ft_strdel(&s_in);
	// }
	// if (s_out)
	// {
	// 	ft_printf("ici\n");
	// 	std[1] = ft_atoi(s_out);
	// 	dup2(std[1], STDOUT_FILENO);
	// 	close(std[1]);
	// 	ft_strdel(&s_out);
	// }
	// if (s_error)
	// {
	// 	ft_printf("ou\n");
	// 	std[2] = ft_atoi(s_error);
	// 	dup2(std[2], STDERR_FILENO);
	// 	close(std[2]);
	// 	ft_strdel(&s_error);
	// }
}
