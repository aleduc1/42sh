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
		ft_dprintf(STDERR_FILENO, "error: tcsetattr\n");
	if (kill(-j->pgid, SIGCONT) < 0)
		ft_dprintf(j->first_process->r->error,
			"42sh: fg: Kill not work!\n");
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
		ft_dprintf(STDERR_FILENO, "error: tcsetpgrp\n");
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
		ft_dprintf(STDERR_FILENO, "error: tcsetpgrp\n");
	if (tcgetattr(shell->term, &j->tmodes) == -1)
		ft_dprintf(STDERR_FILENO, "error: tcgetattr\n");
	if (tcsetattr(shell->term, TCSADRAIN, &(shell->term_shell)) == -1)
		ft_dprintf(STDERR_FILENO, "error: tcsetattr\n");
}

void		add_in_bg(t_job *j, int value)
{
	char	*itoa_pid;

	if (j->pgid < 1)
		exit(-1);
	assign_value_current(j->pgid, j);
	if (tcgetattr(get_shell()->term, &j->tmodes) == -1)
		ft_dprintf(STDERR_FILENO, "error: tcsetpgrp\n");
	itoa_pid = ft_itoa(j->first_process->pid);
	add_set_value("!", itoa_pid);
	if (value && (kill(-j->pgid, SIGCONT) < 0))
		ft_dprintf(j->first_process->r->error, "42sh: bg: Kill not work!\n");
	ft_strdel(&itoa_pid);
}


// void
// add_in_fg(t_job *j, int cont)
// {
// 	t_shell *shell = get_shell();
//   /* Put the job into the foreground.  */
//   tcsetpgrp (shell->term, j->pgid);


//   /* Send the job a continue signal, if necessary.  */
//   if (cont)
//     {
//       tcsetattr (shell->term, TCSADRAIN, &j->tmodes);
//       if (kill (- j->pgid, SIGCONT) < 0)
//         perror ("kill (SIGCONT)");
//     }


//   /* Wait for it to report.  */
//   wait_for_job (j);

//   /* Put the shell back in the foreground.  */
//   tcsetpgrp (shell->term, shell->pgid);

//   /* Restore the shell’s terminal modes.  */
//   tcgetattr (shell->term, &j->tmodes);
//   tcsetattr (shell->term, TCSADRAIN, &shell->term_shell);
//   if (j->pgid < 1)
//  		exit(-1);
// }
