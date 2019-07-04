/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:50:48 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"

int			ft_pipe(char **argv, t_token *token, int end_pipe)
{
	t_job		*j;
	t_process	*p;

	if (end_pipe == 0)
		create_new_job(argv, token);
	else
		add_process(argv, token);
	if (end_pipe == 2)
	{
		j = get_end_job();
		launch_job_pipe(j, 1);
		if (j->first_process->pid == 0)
			clean_fuck_list(0);
		p = j->first_process;
		while (p)
		{
			if (p->completed)
			{
				clean_fuck_list(j->first_process->pid);
				break ;
			}
			p = p->next;
		}
		update_status();
	}
	return (0);
}
