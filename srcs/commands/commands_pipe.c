/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/13 16:40:23 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"

/*
** Check all command in own job and display errors before launch
** Args:	t_job *j -> job to test
*/

static int	check_error_job_pipe(t_job *j)
{
	t_process	*p;
	char		*src;
	int			verif;

	p = j->first_process;
	while (p)
	{
		if (!builtin_exist(p->cmd[0]))
		{
			src = is_in_path(p->cmd[0]);
			if (!src)
				verif = gest_error_path(p->cmd[0], p->r);
			else
			{
				ft_strdel(&src);
				verif = 0;
			}
		}
		else
			verif = 0;
		p = p->next;
	}
	return (verif);
}

/*
** Stock process in job
** Args:	char **argv -> command
**			t_token *token -> struct token
**			int end_pipe -> 0: create a new job, others: add process in last
**					job
**			int fg -> 0: is background process, 1: is foreground process
*/

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
	int			verif;

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
		verif = check_error_job_pipe(j);
		launch_job_pipe(j, fg);
		if (verif != 0)
			gest_return(verif);
	}
	return (0);
}
