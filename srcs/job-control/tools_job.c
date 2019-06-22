/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_job.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 09:24:08 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:21:15 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

t_process	*init_process(void)
{
	t_process	*p;

	if (!(p = (t_process*)malloc(sizeof(t_process) * 1)))
		return (NULL);
	p->cmd_path = 0;
	p->cmd = NULL;
	p->process_id = 0;
	p->pid = 0;
	p->completed = 0;
	p->stopped = 0;
	p->status = 0;
	p->r = NULL;
	p->next = NULL;
	return (p);
}

t_job		*init_job(void)
{
	t_job	*j;

	if (!(j = (t_job*)malloc(sizeof(t_job) * 1)))
		return (NULL);
	j->first_process = init_process();
	j->pgid = 0;
	j->notified = 0;
	j->len_close = 0;
	j->close_fd = NULL;
	j->next = NULL;
	return (j);
}

t_job		**static_job(void)
{
	static t_job	*job;

	if (!job)
		job = init_job();
	return (&job);
}

void		free_process(t_process **p)
{
	if ((!p) || (!(*p)))
		return ;
	if ((*p)->next)
		free_process(&((*p)->next));
	ft_strdel(&((*p)->cmd_path));
	if ((*p)->cmd)
		ft_arraydel(&((*p)->cmd));
	free(*p);
	(*p) = NULL;
}

void		clean_file(t_job *j)
{
	int			i;
	t_process	*p;

	if (j->len_close > 0)
	{
		i = -1;
		while (++i < j->len_close)
			close(j->close_fd[i]);
	}
	p = j->first_process;
	while (p)
	{
		delete_redirection(&p->r);
		p = p->next;
	}
	free(j->close_fd);
	j->close_fd = NULL;
}

void		free_job(t_job **j)
{
	if (j && (*j))
	{
		clean_file(*j);
		free_process(&((*j)->first_process));
		free((*j)->first_process);
		(*j)->first_process = NULL;
		free(*j);
		(*j) = NULL;
	}
}

void		free_all_job(void)
{
	t_job	**j;
	t_job	*h;

	j = static_job();
	while (*j)
	{
		h = (*j)->next;
		// close_file_command((*j)->t->command, &(*j)->r);
		free_job(&(*j));
		(*j) = h;
	}
}

t_job		*get_first_job(t_job *new_job)
{
	t_job	**job;

	job = static_job();
	if (new_job)
		(*job) = new_job;
	return (*job);
}

t_job		*find_job(pid_t pid)
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

int			job_is_stop(t_job *job)
{
	t_process	*p;

	p = job->first_process;
	while (p)
	{
		if ((!p->completed) && (!p->stopped))
			return (0);
		p = p->next;
	}
	return (1);
}

int			job_is_completed(t_job *job)
{
	t_process	*p;

	p = job->first_process;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}
