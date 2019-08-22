/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/22 13:16:25 by sbelondr         ###   ########.fr       */
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

static void	prepare_exec_pipe(int fg)
{
	t_job	*j;

	j = get_end_job();
	j->fg = fg;
	launch_job_pipe(j, fg);
}

/*
** Pipe command
** Args:	char **argv -> command
**			t_token *token -> struct token (contain redirection of file)
**			int end_pipe -> 0: create a new job and add process in this job
**							1: add process in last job
**							2 add process in last job and execute this job
**			int bg -> 0: foreground process, 1: id of background process
*/

int			ft_pipe(char **argv, t_token *token, int end_pipe, int bg)
{
	t_process	*p;
	int			fg;

	if ((!argv) || (!*argv))
		return (-1);
	if (bg != 0)
		manage_id_job(bg);
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
		prepare_exec_pipe(fg);
	return (0);
}
