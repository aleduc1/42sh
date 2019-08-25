/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 23:23:34 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/25 14:08:07 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	delete_redirection(t_redirection **r)
{
	t_redirect	*next;

	next = NULL;
	if ((!r) || (!(*r)))
		return ;
	while ((*r)->redirect)
	{
		next = ((*r)->redirect->next) ? (*r)->redirect->next : NULL;
		if ((*r)->redirect->new_fd != -1
				&& verif_close((*r)->redirect->new_fd))
			close((*r)->redirect->new_fd);
		ft_strdel(&((*r)->redirect->name_file));
		free((*r)->redirect);
		(*r)->redirect = NULL;
		(*r)->redirect = next;
	}
	free(*r);
	(*r) = NULL;
}

int		verif_close(int fd)
{
	int		verif;
	char	*in;
	char	*out;
	char	*error;
	int		fd_sys;

	verif = 1;
	if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO
		|| (fd > -1 && fd < 3))
		return (0);
	in = value_line_path("STDIN", 3);
	out = value_line_path("STDOUT", 3);
	error = value_line_path("STDERR", 3);
	if (in)
	{
		fd_sys = ft_atoi(in);
		if (fd == fd_sys)
			verif = 0;
	}
	if (out)
	{
		fd_sys = ft_atoi(out);
		if (fd == fd_sys)
			verif = 0;
	}
	if (error)
	{
		fd_sys = ft_atoi(error);
		if (fd == fd_sys)
			verif = 0;
	}
	ft_strdel(&in);
	ft_strdel(&out);
	ft_strdel(&error);
	return (verif);
}
