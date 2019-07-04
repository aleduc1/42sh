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

static void	stock_process(char **argv, t_token *token, int end_pipe, int fg)
{
	if (end_pipe == 0)
		create_new_job(argv, token, NULL, fg);
	else
		add_process(argv, token, fg);
}

int			ft_pipe(char **argv, t_token *token, int end_pipe)
{
	t_job		*j;

	stock_process(argv, token, end_pipe, 1);
	if (end_pipe == 2)
	{
		j = get_end_job();
		launch_job_pipe(j, 1);
		clean_fuck_list(0);
		update_status();
	}
	return (0);
}
