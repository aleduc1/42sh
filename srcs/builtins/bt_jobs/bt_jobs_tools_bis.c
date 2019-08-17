/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs_tools_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/17 03:14:16 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"

char	*ft_name_sig(int sig)
{
	char	*str;

	str = NULL;
	if (sig == 0)
		str = ft_strdup("Running");
	else if (sig >= 1 && sig <= 15)
		str = ft_strdup("Terminated");
	else if (sig == 17 || sig == 18 || sig == 21 || sig == 22)
		str = ft_strdup("Stopped");
	else if (sig == 19)
		str = ft_strdup("Running");
	return (str);
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
