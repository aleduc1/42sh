/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 11:34:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/16 03:49:02 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"
#include <errno.h>

void		job_info(t_job *j, char *status)
{
	(void)status;
	if (j->first_process)
		ft_dprintf(j->first_process->r->error, "[%d] %d\n",
				j->process_id, (int)j->pgid);
}

void		job_done(t_job *j)
{
	char	*cmd;
	char	c;

	c = value_char_job(j->current, get_shell()->max_job_current);
	cmd = cmd_job_s(j);
	ft_printf("[%d]%c Done	%s\n", j->process_id, c, cmd);
	j->notif_stop = 1;
	ft_strdel(&cmd);
}

void		display_stat_job(t_job *j)
{
	int			notified;
	t_process	*p;

	notified = 0;
	if (j->first_process->fg == 0)
		if (job_is_completed(j) && j->first_process->status == 0)
		{
			job_done(j);
			return ;
		}
	p = j->first_process;
	while (p)
	{
		if (p->status != p->last_status && p->status != 2)
		{
			if (j->fg == 0 && job_is_completed(j))
				job_done(j);
			else if ((!notified) && (j->fg == 0 || job_is_stopped(j)))
				bt_jobs_s(j, get_shell()->max_job_current, j->r);
			else if (p->status == 3)
			{
				bt_jobs_s(j, get_shell()->max_job_current, j->r);
				break ;
			}
			notified = 1;
			p->last_status = p->status;
		}
		p = p->next;
	}
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
		display_stat_job(j);
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
