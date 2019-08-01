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
** LINE 47:
** else
** 	str = ft_itoa(sig);
*/

char		*ft_inter_signal(int sig)
{
	char	*str;

	str = NULL;
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
		ft_dprintf(r->out, "[%d]%c\t%d Suspended: %d\t%s\n",
			j->first_process->process_id, '+', j->first_process->pid,
			WSTOPSIG(j->first_process->status), j->first_process->cmd[0]);
	else
	{
		ft_dprintf(r->out, "[%d]%c\t%d: %d\t%s\n", j->first_process->process_id,
			'-', j->first_process->pid, WSTOPSIG(j->first_process->status),
			j->first_process->cmd[0]);
	}
}

void		bt_jobs_s(t_job *j, int is_stopped, t_redirection *r)
{
	char	*str;

	str = ft_inter_signal(WSTOPSIG(j->first_process->status));
	if (!str)
		return ;
	if (is_stopped)
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'+', str, j->first_process->cmd[0]);
	else
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'-', str, j->first_process->cmd[0]);
	ft_strdel(&str);
}

static void	display_jobs(void (*p)(t_job*, int, t_redirection*),
	t_redirection *r, char **av)
{
	char	*src;
	t_job	*j;
	int		i;
	int		verif;

	if (!*av)
	{
		j = get_first_job(NULL);
		while (j)
		{
			if (job_is_completed(j))
				(*p)(j, 0, r);
			else if (job_is_stop(j))
				(*p)(j, 1, r);
			j = j->next;
		}
	}
	else
	{
		i = 0;
		if (ft_strequ(*av, "--"))
			++i;
		while (av[i])
		{
			verif = 0;
			j = get_first_job(NULL);
			src = ft_strsub(av[i], 1, ft_strlen(av[i]));
			while (j)
			{
				if (j->first_process->process_id == ft_atoi(src))
				{
					verif = 1;
					if (job_is_completed(j))
						(*p)(j, 0, r);
					else if (job_is_stop(j))
						(*p)(j, 1, r);
					else
						ft_dprintf(r->error, "42sh: jobs %s: no such job\n",
							av[i]);
				}
				j = j->next;
			}
			if (verif == 0)
				ft_dprintf(r->error, "42sh: jobs %s: no such job\n", av[i]);
			ft_strdel(&src);
			++i;
		}
	}
}

int			bt_jobs(char **av, t_redirection *r)
{
	void	(*p)(t_job*, int, t_redirection*);

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
			ft_dprintf(r->error, "42sh: jobs %s: invalid option\n", *av);
			ft_dprintf(r->error, "jobs: usage: jobs [-l | -p] [job_id...]\n");
			return (-2);
		}
	}
	display_jobs(p, r, av);
	return (0);
}

int			process_execute_job(char **av, t_redirection *r, char *name)
{
	int		process;
	int		len;
	char	*src;

	process = -1;
	len = ft_arraylen(av);
	if (len > 2)
	{
		ft_dprintf(r->error, "42sh: %s: Too many argument\n", name);
		return (-1);
	}
	else if (len == 1)
		return (0);
	if (av[1])
	{
		src = (av[1][0] == '%') ? ft_strsub(av[1], 1, ft_strlen(av[1]))
			: ft_strdup(av[1]);
		process = ft_atoi(src);
		ft_strdel(&src);
	}
	return (process);
}

int			bt_bg(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*is_stopped;
	int		process;

	j = get_first_job(NULL);
	is_stopped = NULL;
	if ((process = process_execute_job(av, r, "bg")) < 0)
		return (-2);
	while (j)
	{
		if (j->first_process->stopped == 1
			&& (process == 0 || process == j->first_process->process_id))
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

int			bt_fg(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*is_stopped;
	int		process;

	j = get_first_job(NULL);
	is_stopped = NULL;
	if ((process = process_execute_job(av, r, "fg")) < 0)
		return (-2);
	while (j)
	{
		if (j->first_process->stopped == 1
			&& (process == 0 || process == j->first_process->process_id))
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
