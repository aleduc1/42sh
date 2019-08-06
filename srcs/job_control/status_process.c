/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 11:34:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 12:58:03 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include <errno.h>

void		job_info(t_job *j, char *status)
{
	(void)status;
	if (j->first_process)
		ft_dprintf(j->first_process->r->error, "[%d] %d\n",
			j->first_process->process_id, (int)j->pgid);
}

void		job_notif(void)
{
	t_job	*j;
	t_job	*next;

	update_status();
	j = get_first_job(NULL);
	while (j)
	{
		next = j->next;
		if (j->first_process->fg == 0)
		{
			if (job_is_completed(j))
			{
				if (get_shell()->max_job_current == j->current)
					ft_printf("[%d]%c Done	%s %d\n", j->first_process->process_id,
						'+', j->first_process->cmd[0], j->first_process->status);
				else if (get_shell()->max_job_current - 1 == j->current)
					ft_printf("[%d]%c Done	%s\n", j->first_process->process_id,
						'-', j->first_process->cmd[0]);
				else
					ft_printf("[%d]%c Done	%s\n", j->first_process->process_id,
						' ', j->first_process->cmd[0]);
			}
			j->notif_stop = 1;
		}
		else if (!(j->notif_stop))
		{
			if (job_is_stop(j) && (!j->notified))
			{
				ft_putendl("");
				bt_jobs_s(j, get_shell()->max_job_current, j->r);
			}
			j->notif_stop = 1;
		}
		j = next;
	}
	update_status();
	clean_fuck_list(0);
}

void		job_running(t_job *j)
{
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;
}

void		continue_job(t_job *j, int fg)
{
	job_running(j);
	j->notif_stop = 0;
	if (fg)
		add_in_fg(j, 1);
	else
		add_in_bg(j, 1);
	clean_fuck_list(0);
}
