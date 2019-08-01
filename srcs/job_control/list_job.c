/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_job.c                                         :+:      :+:    :+:   */
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
	p->fg = 0;
	p->return_value = 0;
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
	j->cmd = 0;
	j->pgid = 0;
	j->notified = 0;
	j->notif_stop = 0;
	j->len_close = 0;
	j->r = NULL;
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

t_job		*get_first_job(t_job *new_job)
{
	t_job	**job;

	job = static_job();
	if (new_job)
		(*job) = new_job;
	return (*job);
}

t_job		*create_new_job(char **argv, t_token *t, t_redirection *r, int fg)
{
	t_job			*j;
	t_process		*p;
	int				process_id;

	j = get_first_job(NULL);
	process_id = 0;
	while (j && j->pgid != 0)
	{
		process_id = j->first_process ? j->first_process->process_id : 0;
		if (!j->next)
			j->next = init_job();
		j = j->next;
	}
	file_to_close(t, j);
	j->pgid = 0;
	j->fg = fg;
	j->notif_stop = 0;
	j->r = base_redirection();
	p = j->first_process;
	p->cmd = ft_arraydup(argv);
	parser_var(&p->cmd);
	p->process_id = process_id + 1;
	p->fg = fg;
	p->r = (t) ? fill_redirection(t) : r;
	return (j);
}
