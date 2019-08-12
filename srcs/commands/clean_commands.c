/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:22:31 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"
#include "builtins.h"

void		display_lst_job(t_job *j)
{
	t_job		*sv;
	t_process	*p;

	sv = j;
	if (!sv)
		return ;
	while (sv)
	{
		ft_printf("===========================\n");
		ft_printf("pgpid = %d\nnotified = %d\n", sv->pgid, sv->notified);
		p = sv->first_process;
		while (p)
		{
			ft_printf("___________________________\n");
			ft_arraydisplay(p->cmd);
			ft_printf("pointeur:\n cmd = %p\n cmd_path = %p\n", &p->cmd,
					&p->cmd_path);
			ft_printf("redirection = %p\n", &p->r);
			ft_printf("pid = %d\ncompleted = %d\n", p->pid, p->completed);
			ft_printf("stopped = %d\nstatus = %d\n", p->stopped,
					p->status);
			p = p->next;
		}
		sv = sv->next;
	}
}

static int	condition_clean_list(t_job *j, pid_t pid)
{
	int	status;

	t_process *p;

	p = j->first_process;
	while (p)
	{
		status = p->status;
		if ((!j->notif_stop) && (status == 1 || status == 2 || status == 3 || status == 9
			|| status == 13 || status == 15))
		{
			ft_dprintf(STDERR_FILENO, "[1]+  Terminated: %d          %s\n",
				j->first_process->status, j->first_process->cmd[0]);
			j->notif_stop = 1;
		}
		p = p->next;
	}
	// ft_printf("%d, %d - %d\n", status, job_is_completed(j), job_is_stopped(j));
	if (job_is_completed(j) || (j->first_process->pid == 0
		&& job_is_stopped(j) == 0)
		|| j->first_process->pid == pid)
		return (1);
	return (0);
}

void		clean_fuck_list(pid_t pid)
{
	t_job	**j;
	t_job	*last;
	t_job	*next;
	t_job	*h;

	update_status();
	j = static_job();
	h = *j;
	last = NULL;
	while (*j)
	{
		next = (*j)->next;
		if (condition_clean_list(*j, pid))
		{
			if (last)
				last->next = next;
			else
				h = next;
			free_job(&(*j));
		}
		else
			last = (*j);
		(*j) = next;
	}
	(*j) = h;
}

static void	free_process_redirection(t_job **j)
{
	t_process	*p;
	t_process	*next;

	if ((!j) || (!(*j)))
		return ;
	p = (*j)->first_process;
	while (p)
	{
		next = p->next;
		ft_strdel(&(p->cmd_path));
		if (p->cmd)
			ft_arraydel(&(p->cmd));
		free(p);
		p = NULL;
		p = next;
	}
}

void		free_job_redirection(t_job **j)
{
	if (j && (*j))
	{
		if ((*j)->first_process)
			free_process_redirection(j);
		free(*j);
		(*j) = NULL;
	}
}
