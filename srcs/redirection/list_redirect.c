/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 23:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/25 05:11:36 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_redirect	*ft_init_redirect(void)
{
	t_redirect	*r;

	if (!(r = (t_redirect*)malloc(sizeof(t_redirect) * 1)))
		return (NULL);
	r->base = -1;
	r->new_fd = -1;
	r->type = -1;
	r->name_file = 0;
	r->next = NULL;
	return (r);
}

int			ft_create_maillon_redirect_env(t_redirect *r, int base, int new_fd,
				char *name, int type)
{
	t_redirect	*last;

	last = NULL;
	if (r)
		while (r && r->base != -1)
		{
			last = r;
			r = r->next;
		}
	if (!r)
		r = ft_init_redirect();
	if (r)
	{
		r->base = base;
		r->new_fd = new_fd;
		r->name_file = ft_strdup(name);
		r->type = type;
		if (last)
			last->next = r;
		return (0);
	}
	return (-1);
}

int			ft_create_maillon_redirect(t_redirect *r, int base, int new_fd,
				t_redir *redir)
{
	t_redirect	*last;

	last = NULL;
	if (r)
		while (r && r->base != -1)
		{
			last = r;
			r = r->next;
		}
	if (!r)
		r = ft_init_redirect();
	if (r)
	{
		r->base = base;
		r->new_fd = new_fd;
		r->name_file = (redir) ? ft_strdup(redir->filename) : 0;
		r->type = (redir) ? redir->type : -1;
		if (last)
			last->next = r;
		return (0);
	}
	return (-1);
}

int			ft_fd_redirect_exist(t_redirect *r, int base)
{
	t_redirect	*head;

	if (!r)
		return (0);
	head = r;
	while (r)
	{
		if (r->base == base)
			return (1);
		if (!(r->next))
			return (0);
		r = r->next;
	}
	r = head;
	return (0);
}
