/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 12:56:32 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

/*
** WTERMSIG(j->first_process->status));
*/

char		*ft_inter_signal(int sig)
{
	char	*str;

	if (sig == 1)
		str = ft_strdup("Terminated(SIGHUP)");
	else if (sig == 2)
		str = ft_strdup("Terminated(SIGINT)");
	else if (sig == 3)
		str = ft_strdup("Terminated(SIGQUIT)");
	else if (sig == 9)
		str = ft_strdup("Terminated(SIGKILL)");
	else if (sig == 13)
		str = ft_strdup("Terminated(SIGPIPE)");
	else if (sig == 15)
		str = ft_strdup("Terminated(SIGTERM)");
	else if (sig == 17)
		str = ft_strdup("Stopped(SIGSTOP)");
	else if (sig == 18)
		str = ft_strdup("Stopped(SIGTSTP)");
	else if (sig == 21)
		str = ft_strdup("Stopped(SIGTTIN)");
	else if (sig == 22)
		str = ft_strdup("Stopped(SIGTTOU)");
	else
		str = ft_itoa(sig);
	return (str);
}

static void	bt_jobs_p(t_job *j, int is_stopped, t_redirection *r)
{
	if (is_stopped)
		ft_dprintf(r->out, "%d\n", j->first_process->pid);
	else
		ft_dprintf(r->out, "%d\n", j->first_process->pid);
}

static void	bt_jobs_l(t_job *j, int is_stopped, t_redirection *r)
{
	if (is_stopped)
		ft_dprintf(r->out, "[%d]%c\t%d Suspended: %d\t%s\n", j->first_process->process_id,
			'+', j->first_process->pid, WSTOPSIG(j->first_process->status),
			j->first_process->cmd[0]);
	else
	{
		ft_dprintf(r->out, "[%d]%c\t%d: %d\t%s\n", j->first_process->process_id,
			'-', j->first_process->pid, WSTOPSIG(j->first_process->status),
			j->first_process->cmd[0]);
	}
}

static void	bt_jobs_s(t_job *j, int is_stopped, t_redirection *r)
{
	char	*str;

	str = ft_inter_signal(WSTOPSIG(j->first_process->status));
	if (is_stopped)
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'+', str, j->first_process->cmd[0]);
	else
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'-', str, j->first_process->cmd[0]);
	ft_strdel(&str);
}

int		bt_jobs(char **av, t_redirection *r)
{
	t_job	*j;
	void	(*p)(t_job*, int, t_redirection*);


	update_status();
	// display_redirection(r);
	p = &bt_jobs_s;
	while (*(++av))
	{
		if (ft_strequ(*av, "-p"))
			p = &bt_jobs_p;
		else if (ft_strequ(*av, "-l"))
			p = &bt_jobs_l;
		else
		{
			ft_dprintf(r->error, "42sh: jobs %s: invalid option\n", *av);
			ft_dprintf(r->error, "jobs: usage: jobs [-l | -p] [job_id...]\n");
			return (-2);
		}
	}
	j = get_first_job(NULL);
	while (j)
	{
		if (job_is_completed(j))
			(*p)(j, 0, r);
		else if (job_is_stop(j) && (!j->notified))
			(*p)(j, 1, r);
		j = j->next;
	}
	return (0);
}

int		bt_bg(void)
{
	t_job	*j;
	t_job	*is_stopped;

	j = get_first_job(NULL);
	is_stopped = NULL;
	while (j)
	{
		if (j->first_process->stopped == 1)
			is_stopped = j;
		j = j->next;
	}
	if (!is_stopped)
	{
		ft_dprintf(2, "42sh: bg no current job\n");
		return (-2);
	}
	continue_job(is_stopped, 0);
	return (0);
}

int		bt_fg(void)
{
	t_job	*j;
	t_job	*is_stopped;

	j = get_first_job(NULL);
	is_stopped = NULL;
	while (j)
	{
		if (j->first_process->stopped == 1)
			is_stopped = j;
		j = j->next;
	}
	if (!is_stopped)
	{
		ft_dprintf(2, "42sh: fg: no current job\n");
		return (-2);
	}
	continue_job(is_stopped, 1);
	return (0);
}
