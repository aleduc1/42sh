/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_fg_bg_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/17 01:56:38 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"

static t_job	*job_for_bg_fg_opt(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*job_launch;
	int		i;

	j = get_first_job(NULL);
	job_launch = NULL;
	i = 1;
	if (ft_strequ(*av, "--"))
		++i;
	while (av[i])
	{
		j = search_job(av[i]);
		if (j)
		{
			if (job_is_completed(j) || job_is_stopped(j) || j->pgid > 0)
				job_launch = j;
			else
				display_no_such_job(r, av[i]);
		}
		else
			display_no_such_job(r, av[i]);
		++i;
	}
	return (job_launch);
}

t_job			*job_for_bg_fg(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*job_launch;

	update_status();
	if (!av[1])
	{
		j = get_first_job(NULL);
		job_launch = NULL;
		while (j)
		{
			if (j->first_process->stopped == 1 || j->pgid > 0)
				job_launch = j;
			j = j->next;
		}
	}
	else
		job_launch = job_for_bg_fg_opt(av, r);
	return (job_launch);
}

void			change_fg_var_job(t_job *j, int fg)
{
	t_process	*p;

	j->fg = fg;
	p = j->first_process;
	while (p)
	{
		p->fg = fg;
		p = p->next;
	}
}
