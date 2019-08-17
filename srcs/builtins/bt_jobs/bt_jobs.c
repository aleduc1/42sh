/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/17 01:56:38 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"

static void	display_jobs(void (*p)(t_job*, int, t_redirection*),
	t_redirection *r, char **av)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current;
	if (!*av)
	{
		j = get_first_job(NULL);
		while (j)
		{
			if (job_is_completed(j) || job_is_stopped(j) || j->pgid > 0)
				(*p)(j, max_current, r);
			j = j->next;
		}
	}
	else
		display_jobs_options(p, r, av);
}

int			bt_jobs(t_job *j, char **av, t_redirection *r)
{
	void	(*p)(t_job*, int, t_redirection*);

	if (j->fg == 0)
	{
		display_no_job_control(r, "jobs");
		return (1);
	}
	update_status();
	p = &bt_jobs_s;
	while (*(++av))
	{
		if (ft_strequ(*av, "-p"))
			p = &bt_jobs_p;
		else if (ft_strequ(*av, "-l"))
			p = &bt_jobs_l;
		else if (ft_strequ(*av, "--") || ft_strchr_exist(*av, '%'))
			break ;
		else
		{
			display_invalid_option_job(r, *av);
			return (-2);
		}
	}
	display_jobs(p, r, av);
	return (0);
}
