/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/19 18:06:03 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"

static void	standard_redirection(t_redirection *r)
{
	if (r->in != STDIN_FILENO
		&& ft_fd_redirect_exist(r->redirect, STDIN_FILENO) == 0)
	{
		dup2(r->in, STDIN_FILENO);
		close(r->in);
	}
	if (r->out != STDOUT_FILENO
		&& ft_fd_redirect_exist(r->redirect, STDOUT_FILENO) == 0)
	{
		dup2(r->out, STDOUT_FILENO);
		close(r->out);
	}
	if (r->error != STDERR_FILENO
		&& ft_fd_redirect_exist(r->redirect, STDERR_FILENO) == 0)
	{
		dup2(r->error, STDERR_FILENO);
		close(r->error);
	}
}

static void	custom_redirection(t_redirection *r, t_redirect *lst)
{
	if (lst->base != -1)
	{
		if (STDIN_FILENO == lst->base)
			redir_in(r);
		else if (STDOUT_FILENO == lst->base)
			redir_out(r);
		else if (STDERR_FILENO == lst->base)
			redir_error(r);
		else
			other_redir(lst->base, lst->new_fd);
	}
}

void		redirection_fd(t_redirection *r)
{
	t_redirect	*lst;

	lst = r->redirect;
	while (lst)
	{
		custom_redirection(r, lst);
		lst = lst->next;
	}
	standard_redirection(r);
	if (verif_close(r->in) && ft_fd_redirect_exist(r->redirect, STDIN_FILENO))
		close(r->in);
	if (verif_close(r->out) && ft_fd_redirect_exist(r->redirect, STDOUT_FILENO))
		close(r->out);
	if (verif_close(r->error)
		&& ft_fd_redirect_exist(r->redirect, STDERR_FILENO))
		close(r->error);
}
