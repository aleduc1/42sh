/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 23:23:34 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/17 14:02:50 by sbelondr         ###   ########.fr       */
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
		free((*r)->redirect);
		(*r)->redirect = NULL;
		(*r)->redirect = next;
	}
	free(*r);
	(*r) = NULL;
}

int		verif_close(int fd)
{
	if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO
		|| (fd > -1 && fd < 3))
		return (0);
	return (1);
}
