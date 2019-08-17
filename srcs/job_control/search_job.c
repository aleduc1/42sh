/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/17 01:56:38 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

t_job	*last_jobs(void)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current;
	j = get_first_job(NULL);
	while (j)
	{
		if (j->current == max_current)
			return (j);
		j = j->next;
	}
	return (NULL);
}

t_job	*previous_jobs(void)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current - 1;
	j = get_first_job(NULL);
	while (j)
	{
		if (j->current == max_current)
			return (j);
		j = j->next;
	}
	return (NULL);
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
		if (job->process_id == id)
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
