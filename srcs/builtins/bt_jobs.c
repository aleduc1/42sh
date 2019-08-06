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

char		*ft_inter_signal(int sig, t_job *j)
{
	char	*str;

	str = NULL;
	if (sig == 0 && job_is_completed(j))
		str = ft_strdup("Done");
	else if (sig == 0 && (!job_is_completed(j)))
		str = ft_strdup("Running");
	else if (sig == 1)
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

void		bt_jobs_p(t_job *j, int max_current, t_redirection *r)
{
	t_process	*p;

	(void)max_current;
	p = j->first_process;
	ft_dprintf(r->out, "[%d]", p->process_id);
	while (p)
	{
		ft_dprintf(r->out, " %d", p->pid);
		p = p->next;
	}
	ft_dprintf(r->out, "\n");
}

static void	bt_jobs_l(t_job *j, int max_current, t_redirection *r)
{
	if (j->current == max_current)
		ft_dprintf(r->out, "[%d]%c\t%d Suspended: %d\t%s\n",
			j->first_process->process_id, '+', j->first_process->pid,
			WSTOPSIG(j->first_process->status), j->first_process->cmd[0]);
	else if (j->current == max_current - 1)
		ft_dprintf(r->out, "[%d]%c\t%d: %d\t%s\n", j->first_process->process_id,
			'-', j->first_process->pid, WSTOPSIG(j->first_process->status),
			j->first_process->cmd[0]);
	else
		ft_dprintf(r->out, "[%d]%c\t%d: %d\t%s\n", j->first_process->process_id,
			' ', j->first_process->pid, WSTOPSIG(j->first_process->status),
			j->first_process->cmd[0]);
}

void		bt_jobs_s(t_job *j, int max_current, t_redirection *r)
{
	char	*str;

	str = ft_inter_signal(WSTOPSIG(j->first_process->status), j);
	// if ((!str) && job_is_stop(j) == 0 && job_is_completed(j) == 0)
	// 	str = ft_strdup("Running");
	if (!str)
		return ;
	if (j->current == max_current)
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'+', str, j->first_process->cmd[0]);
	else if (j->current == max_current - 1)
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			'-', str, j->first_process->cmd[0]);
	else
		ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->first_process->process_id,
			' ', str, j->first_process->cmd[0]);
	ft_strdel(&str);
}

static pid_t	last_jobs(void)
{
	t_job	*j;
	pid_t	pid;

	pid = -1;
	j = get_first_job(NULL);
	while (j)
	{
		if (job_is_completed(j) || job_is_stop(j))
			pid = j->pgid;
		j = j->next;
	}
	return (pid);
}

static pid_t	previous_jobs(void)
{
	t_job	*j;
	pid_t	pid;
	pid_t	last;

	pid = -1;
	last = -1;
	j = get_first_job(NULL);
	while (j)
	{
		if (job_is_completed(j) || job_is_stop(j))
		{
			last = pid;
			pid = j->pgid;
		}
		j = j->next;
	}
	return (last);
}

t_job	*ft_search_pid_job(pid_t pid)
{
	t_job	*job;

	job = get_first_job(NULL);
	while (job)
	{
		if (job->pgid == pid)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*ft_search_id_job(char *av, int index)
{
	t_job	*job;
	int		id;

	id = ft_atoi(av + index);
	job = get_first_job(NULL);
	while (job)
	{
		if (job->first_process->process_id == id)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*ft_search_str_job(char *av, int index)
{
	t_job	*job;
	t_job	*final;
	char	*str;

	job = get_first_job(NULL);
	final = NULL;
	while (job)
	{
		str = ft_strnstr(job->first_process->cmd[0], av + index,
				ft_strlen(av + index));
		if (str)
		{
			if (final)
				return (NULL);
			else
				final = job;
		}
		job = job->next;
	}
	return (final);
}

t_job	*ft_search_exist_job(char *av, int index)
{
	t_job	*job;
	t_job	*final;
	char	*str;
	int		id;

	id = ft_atoi(av + index);
	job = get_first_job(NULL);
	final = NULL;
	while (job)
	{
		str = ft_strstr(job->first_process->cmd[0], av + index + 1);
		if (str)
		{
			if (final)
				return (NULL);
			else
				final = job;
		}
		job = job->next;
	}
	return (final);
}

static int		ft_good_syntax(char *av)
{
	int	i;
	int	percent;

	i = 0;
	if (av[i] == '%')
		++i;
	percent = (i > 0) ? -1 : 1;
	if (!av[i])
		return (4);
	if (av[i] == '%' || av[i] == '+')
		return ((!av[i + 1]) ? 1 * percent : 4 * percent);
	if (av[i] == '-')
		return ((!av[i + 1]) ? 2 * percent : 4 * percent);
	if (av[i] == '?')
		return (5 * percent);
	return (ft_isnumbers(av + i) ? 3 * percent : 4 * percent);
}

static t_job	*search_job(char *av)
{
	int	job_id;

	job_id = ft_good_syntax(av);
	if (job_id == 1 || job_id == -1)
		return (ft_search_pid_job(last_jobs()));
	else if (job_id == 2 || job_id == -2)
		return (ft_search_pid_job(previous_jobs()));
	else if (job_id == 3 || job_id == -3)
		return (ft_search_id_job(av, (job_id > -1) ? 0 : 1));
	else if (job_id == 4 || job_id == -4)
		return (ft_search_str_job(av, (job_id > -1) ? 0 : 1));
	else if (job_id == 5 || job_id == -5)
		return (ft_search_exist_job(av, (job_id > -1) ? 0 : 1));
	else
		return (NULL);
}

static void	display_jobs(void (*p)(t_job*, int, t_redirection*),
	t_redirection *r, char **av)
{
	t_job	*j;
	int		i;
	int		verif;
	int		max_current;

	max_current = get_shell()->max_job_current;
	if (!*av)
	{
		j = get_first_job(NULL);
		while (j)
		{
			if (job_is_completed(j) || job_is_stop(j) || j->pgid > 0)
					(*p)(j, max_current, r);
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
			j = search_job(av[i]);
			if (j)
			{
				verif = 1;
				if (job_is_completed(j) || job_is_stop(j))
						(*p)(j, max_current, r);
				else
					display_no_such_job(r, av[i]);
			}
			else
				display_no_such_job(r, av[i]);
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
			display_invalid_option_job(r, *av);
			return (-2);
		}
	}
	display_jobs(p, r, av);
	return (0);
}

t_job		*job_for_bg_fg(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*job_launch;
	int		i;
	pid_t	current_pid;
	pid_t	previous_pid;

	update_status();
	current_pid = last_jobs();
	previous_pid = previous_jobs();
	j = get_first_job(NULL);
	job_launch = NULL;
	if (!av[1])
	{
		while (j)
		{
			if (j->first_process->stopped == 1 || j->pgid > 0)
				job_launch = j;
			j = j->next;
		}
	}
	else
	{
		i = 1;
		if (ft_strequ(*av, "--"))
			++i;
		while (av[i])
		{
			j = search_job(av[i]);
			if (j)
			{
				if (job_is_completed(j) || job_is_stop(j) || j->pgid > 0)
					job_launch = j;
				else
					display_no_such_job(r, av[i]);
			}
			else
				display_no_such_job(r, av[i]);
			++i;
		}
	}
	return (job_launch);
}

int			bt_bg(char **av, t_redirection *r)
{
	t_job	*job_launch;

	job_launch = job_for_bg_fg(av, r);
	if (!job_launch)
	{
		display_no_current_job(r, "bg");
		return (-2);
	}
	continue_job(job_launch, 0);
	return (0);
}

int			bt_fg(char **av, t_redirection *r)
{
	t_job	*job_launch;

	job_launch = job_for_bg_fg(av, r);
	if (!job_launch)
	{
		display_no_current_job(r, "fg");
		return (-2);
	}
	continue_job(job_launch, 1);
	return (0);
}
