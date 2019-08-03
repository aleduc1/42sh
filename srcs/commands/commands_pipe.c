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

int			ft_pipe(char **argv, t_token *token, int end_pipe, int bg)
{
	t_job		*j;
	t_process	*p;
	int			fg;

	fg = (bg > 0) ? 0 : 1;
	stock_process(argv, token, end_pipe, fg);
	if (check_last_command() == -5)
	{
		p = get_end_job()->first_process;
		while (p->next)
			p = p->next;
		p->return_value = 1;
		gest_return(1);
	}
	if (end_pipe == 2)
	{
		j = get_end_job();
		j->fg = fg;
		launch_job_pipe(j, fg);
	}
	return (0);
}
