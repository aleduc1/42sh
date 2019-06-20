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

int			ft_pipe(char **argv, t_token *t, int end_pipe)
{
	t_job	*j;

	if (end_pipe == 0)
		create_new_job(argv, t);
	if (end_pipe > 0)
		add_process(argv, t);
	if (end_pipe == 2)
	{
		j = get_end_job();
		launch_job_pipe(j, 1);
		if (j->first_process->completed == 1 || j->first_process->pid == 0)
			clean_fuck_list();
	}
	return (0);
}
