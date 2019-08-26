/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 11:34:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/26 06:24:10 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"
#include <errno.h>

void	job_done(t_job *j)
{
	char	*cmd;
	char	*cache;
	char	*stock;
	char	c;
	int		verif;
	int		id;
	t_job	*lst;

	lst = get_first_job(NULL);
	id = j->process_id;
	verif = 1;
	while (lst)
	{
		if (id == lst->process_id && j->pgid != lst->pgid)
			verif = (job_is_completed(lst)) ? 1 : 0;
		lst = lst->next;
	}
	if (verif == 1 && j->notified == 0)
	{
		c = value_char_job(j->current, get_shell()->max_job_current);
		cmd = ft_strdup("");
		lst = get_first_job(NULL);
		while (lst)
		{
			if (id == lst->process_id)
			{
				cache = cmd_job_s(lst);
				stock = ft_strjoin(cmd, cache);
				ft_strdel(&cache);
				ft_strdel(&cmd);
				cmd = stock;
				lst->notified = 1;
			}
			lst = lst->next;
		}
		ft_dprintf(STDOUT_FILENO, "[%d]%c\tDone\t%s\n", j->process_id, c, cmd);
		ft_strdel(&cmd);
	}
}

static int	check_is_terminated(t_job *j)
{
	int			num_sig;
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		num_sig = (p->status < 32) ? p->status : WSTOPSIG(p->status);
		if (num_sig < 13 && num_sig > 15)
			return (0);
		p = p->next;
	}
	return (0);
}

static int	display_stat_process(t_job *j, t_process *p, int notified)
{
	if ((!notified) && (p->status != p->last_status && p->status != 2))
	{
		if (j->fg == 0 && job_is_completed(j))
			bt_jobs_s(j, get_shell()->max_job_current);
		else if ((!notified) && (j->fg == 1 && job_is_stopped(j)))
		{
			ft_putchar('\n');
			bt_jobs_s(j, get_shell()->max_job_current);
		}
		notified = 1;
	}
	p->last_status = p->status;
	return (notified);
}

/*
** Parcours chaque process d'un job et utilise la fonction
**	display_stat_process pour afficher le status d'un process
**	si besoin
** Args:	t_job *j -> process a check
*/

static void	display_stat_job(t_job *j)
{
	int			notified;
	t_process	*p;

	notified = 0;
	if (check_is_terminated(j))
	{
		if (j->first_process->status != 0)
			bt_jobs_s(j, get_shell()->max_job_current);
		return ;
	}
	if (j->first_process->fg == 0)
		if (job_is_completed(j))// && j->first_process->status == 0)
		{
			job_done(j);
			return ;
		}
	p = j->first_process;
	while (p)
	{
		if (!j->notified)
			j->notified = display_stat_process(j, p, j->notified);
		p = p->next;
	}
}

void		job_notif(void)
{
	t_process	*p;
	t_job	*j;


	update_status();
	j = get_first_job(NULL);
	while (j)
	{
		p = j->first_process;
		while (p)
		{
			p->status = convert_value_signal(p->status);
			p->last_status = p->status;
			p = p->next;
		}
		j = j->next;
	}
/*	j = get_first_job(NULL);
	while (j)
	{
		display_stat_job(j);
		j = j->next;
	}*/
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
