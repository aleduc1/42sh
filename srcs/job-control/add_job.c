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

/*
** TCSADRAIN
** The change occurs after all output written to fildes has been transmitted to
** the terminal.
** kill(-pgid, SIG) -> kill un grp d'id
*/

void	add_in_fg(t_job *j, int value)
{
	t_shell *s = get_shell();
	 /* Put the job into the foreground.  */
  tcsetpgrp (g_in, j->pgid);


  /* Send the job a continue signal, if necessary.  */
  if (value)
    {
      tcsetattr (g_in, TCSADRAIN, &j->tmodes);
      if (kill (- j->pgid, SIGCONT) < 0)
        perror ("kill (SIGCONT)");
    }


  /* Wait for it to report.  */
  wait_for_job (j);

  /* Put the shell back in the foreground.  */
  tcsetpgrp (g_in, s->pgid);

  /* Restore the shell’s terminal modes.  */
  tcgetattr (g_in, &j->tmodes);
  tcsetattr (g_in, TCSADRAIN, &s->term_shell);

	/*
	t_shell	*shell;

	shell = get_shell();
	tcsetpgrp(shell->term, j->pgid);
	if (value)
	{
		tcsetattr(shell->term, TCSADRAIN, &(j->tmodes));
		if (kill(-j->pgid, SIGCONT) < 0)
			ft_dprintf(j->first_process->r->error, "42sh: fg: Kill not work!\n");
	}
	wait_for_job(j);
	tcsetpgrp(shell->term, shell->pgid);
	tcgetattr(shell->term, &j->tmodes);
	tcsetattr(shell->term, TCSADRAIN, &(shell->term_shell));*/
}

void	add_in_bg(t_job *j, int value)
{
	t_shell	*shell;
	char	*itoa_pid;

	shell = get_shell();
	itoa_pid = ft_itoa(j->first_process->pid);
	add_set_value("!", itoa_pid);
	if (value && (kill(-j->pgid, SIGCONT) < 0))
		ft_dprintf(j->first_process->r->error, "42sh: bg: Kill not work!\n");
}
