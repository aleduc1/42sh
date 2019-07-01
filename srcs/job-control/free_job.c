/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_job.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 09:24:08 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:21:15 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

void		free_process(t_process **p)
{
	if ((!p) || (!(*p)))
		return ;
	if ((*p)->next)
		free_process(&((*p)->next));
	ft_strdel(&((*p)->cmd_path));
	if ((*p)->cmd)
		ft_arraydel(&((*p)->cmd));
	free(*p);
	(*p) = NULL;
}

void		clean_file(t_job *j)
{
	int			i;
	t_process	*p;

	if (j->len_close > 0)
	{
		i = -1;
		while (++i < j->len_close)
			close(j->close_fd[i]);
	}
	p = j->first_process;
	while (p)
	{
		delete_redirection(&p->r);
		p = p->next;
	}
	free(j->close_fd);
	j->close_fd = NULL;
}

void		free_redirection(t_redirection **r)
{
	free(*r);
	(*r) = NULL;
}

void		free_job(t_job **j)
{
	if (j && (*j))
	{
		clean_file(*j);
		free_process(&((*j)->first_process));
		free((*j)->first_process);
		(*j)->first_process = NULL;
		free_redirection(&((*j)->r));
		free(*j);
		(*j) = NULL;
	}
}

void		free_all_job(void)
{
	t_job	**j;
	t_job	*h;

	j = static_job();
	while (*j)
	{
		h = (*j)->next;
		free_job(&(*j));
		(*j) = h;
	}
}
