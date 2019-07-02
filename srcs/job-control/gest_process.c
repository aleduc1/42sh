/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gest_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 11:34:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 12:58:03 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

#include <errno.h>

/*
** stock le status du pid
** WIFSIGNALED -> check if process exist
** WIFSTOPPED -> True if the process has
** not terminated, but has stopped and can be restarted.
*/

/*
** if (WIFSIGNALED(status))
**	ft_dprintf(p->r->error, "\nProcess end pid = %d %d",
**		(int)pid, status);
*/

static int	action_process_status(pid_t pid, int status, t_job *j, t_process *p)
{
	if (p->pid == pid)
	{
		p->status = status;
		if (WIFCONTINUED(status))
		{
			p->stopped = 0;
			j->notif_stop = 0;
		}
		if (WIFSTOPPED(status))
		{
			if (j->notif_stop == 0)
				ft_printf("\n[1]+  Stopped(SIGTSTP)\t%s\n", p->cmd[0]);
			p->stopped = 1;
			j->notif_stop = 1;
		}
		else
		{
			p->completed = 1;
			j->notif_stop = 0;
			if (WIFSIGNALED(status))
			{
				gest_return(WTERMSIG(p->status));
			}
		}
		return (0);
	}
	return (1);
}

int			mark_process_status(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if (pid > 0)
	{
		j = get_first_job(NULL);
		while (j)
		{
			p = j->first_process;
			while (p)
			{
				if (action_process_status(pid, status, j, p) == 0)
					return (0);
				p = p->next;
			}
			j = j->next;
		}
		ft_dprintf(STDERR_FILENO, "No child process %d.\n", pid);
		return (-1);
	}
	return (-1);
}

/*
** WNOHANG: revenir immédiatement si aucun fils n'est achevé.
*/

void		update_status(void)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG | WCONTINUED);
		if (mark_process_status(pid, status))
			break ;
	}
}

void		wait_for_job(t_job *j)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (mark_process_status(pid, status) || job_is_stop(j)
			|| job_is_completed(j))
			break ;
	}
}

void		job_info(t_job *j, char *status)
{
	if (j->first_process)
	ft_dprintf(j->first_process->r->error, "%s [%d]: %s\n",
		j->first_process->cmd[0], (int)j->pgid, status);
}

/*
** supprimer?
*/

void		notif_stop(t_job *j)
{
	job_info(j, "stopped");
	j->notified = 1;
}

void		job_notif(void)
{
	t_job	*j;
	t_job	*last;
	t_job	*next;

	update_status();
	j = get_first_job(NULL);
	last = NULL;
	while (j)
	{
		next = j->next;
		if (job_is_completed(j))
		{
			job_info(j, "completed");
			if (last)
				last->next = next;
			else if (next)
				get_first_job(next);
			else
			{
				free_job(&j);
				next = init_job();
				get_first_job(next);
				break ;
			}
			free_job(&j);
		}
		else if (job_is_stop(j) && !j->notified)
		{
			notif_stop(j);
			last = j;
		}
		else
			last = j;
		j = next;
	}
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
	if (!j)
	{
		j = init_job();
		get_first_job(j);
	}
}
