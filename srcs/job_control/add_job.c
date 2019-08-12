/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_job.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 13:59:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 12:57:15 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "sh21.h"

void		edit_current_value(int value)
{
	t_job	*j;

	j = get_first_job(NULL);
	while (j)
	{
		if (j->current > value)
			j->current -= 1;
		j = j->next;
	}
}

static void	assign_value_current(pid_t pgid, t_job *new_job)
{
	t_job	*j;
	int		max;
	int		min;

	if (new_job->current == 0)
	{
		get_shell()->max_job_current += 1;
		new_job->current = get_shell()->max_job_current;
	}
	j = get_first_job(NULL);
	max = get_shell()->max_job_current;
	while (j)
	{
		if (j->pgid == pgid)
		{
			min = j->current;
			j->current = max + 1;
		}
		j = j->next;
	}
	edit_current_value(min);
}

/*
** TCSADRAIN
** The change occurs after all output written to fildes has been transmitted to
** the terminal.
** kill(-pgid, SIG) -> kill un grp d'id
** if (tcsetattr(shell->term, TCSADRAIN, &(j->tmodes)) == -1)
*/

static void	kill_pgid(t_shell *shell, t_job *j)
{
	if (tcsetattr(shell->term, TCSADRAIN, &(j->tmodes)) == -1)
		display_error_tc(j->r, "tcsetattr");
	if (kill(-j->pgid, SIGCONT) < 0)
		display_kill_not_work(j->r, "fg");
}

void		add_in_fg(t_job *j, int value)
{
	t_shell		*shell;
	t_process	*p;

	if (j->pgid < 1)
		exit(-1);
	assign_value_current(j->pgid, j);
	p = j->first_process;
	shell = get_shell();
	if (tcsetpgrp(shell->term, j->pgid) == -1)
		display_error_tc(j->r, "tcsetpgrp");
	if (value)
		kill_pgid(shell, j);
	while (p)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;
	wait_for_job(j);
	if (tcsetpgrp(shell->term, shell->pgid) == -1)
		display_error_tc(j->r, "tcsetpgrp");
	if (tcgetattr(shell->term, &j->tmodes) == -1)
		display_error_tc(j->r, "tcgetattr");
	if (tcsetattr(shell->term, TCSADRAIN, &(shell->term_shell)) == -1)
		display_error_tc(j->r, "tcsetattr");
}

void		add_in_bg(t_job *j, int value)
{
	char		*itoa_pid;
	t_process	*p;

	if (j->pgid < 1)
		exit(-1);
	assign_value_current(j->pgid, j);
	if (tcgetattr(get_shell()->term, &j->tmodes) == -1)
		display_error_tc(j->r, "tcsetattr");
	p = j->first_process;
	while (p)
	{
		if (!p->next)
			break ;
		p = p->next;
	}
	itoa_pid = ft_itoa(p->pid);
	add_set_value("!", itoa_pid);
	if (value)
		if (kill(-j->pgid, SIGCONT) < 0)
			display_kill_not_work(j->r, "bg");
	ft_strdel(&itoa_pid);
}
