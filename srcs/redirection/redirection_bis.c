/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/19 18:06:24 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/*
** in -> STDIN_FILENO
** out -> STDOUT_FILENO
** error -> STDERR_FILLENO
*/

void	redir_in(t_redirection *r)
{
	if (r->in != STDIN_FILENO)
		dup2(r->in, STDIN_FILENO);
}

void	redir_out(t_redirection *r)
{
	if (r->out != STDOUT_FILENO)
		dup2(r->out, STDOUT_FILENO);
}

void	redir_error(t_redirection *r)
{
	if (r->error != STDERR_FILENO)
		dup2(r->error, STDERR_FILENO);
}

/*
** dup2(src, new_fd);
** if (verif_close(src))
**	close(src);
*/

void	other_redir(int src, int new_fd)
{
	if (new_fd == -1)
		return ;
	if (src != new_fd)
	{
		dup2(new_fd, src);
		if (verif_close(new_fd))
			close(new_fd);
	}
}
