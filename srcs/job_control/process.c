/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 09:42:05 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/05 02:17:20 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "job.h"
#include "builtins.h"

int			launch_process(t_process *p, pid_t pgid, int fg)
{
	char	**environ;
	int		verif;

	environ = create_list_env(get_env(0, NULL), 1);
	config_pid_process(pgid, fg);
	redirection_fd(p->r);
	verif = execve(p->cmd_path, p->cmd, environ);
	display_command_not_found(p->r, p->cmd[0]);
	execve("/bin/test", NULL, NULL);
	exit(verif);
}

void		display_redirection(t_redirection *r)
{
	ft_printf("in = %d\n", r->in);
	ft_printf("out = %d\n", r->out);
	ft_printf("error = %d\n", r->error);
	while (r->redirect)
	{
		ft_printf("base = %d, new_fd = %d\n",
			r->redirect->base, r->redirect->new_fd);
		r->redirect = r->redirect->next;
	}
}

int			launch_job(t_job *j, int fg)
{
	t_process	*p;
	pid_t		pid;
	int			verif;

	p = j->first_process;
	verif = 0;
	while (p)
	{
		pid = fork();
		if (pid == 0)
		{
			if (p->cmd_path)
				verif = launch_process(p, j->pgid, fg);
			else
				execve("/bin/test", NULL, NULL);
		}
		else if (pid < 0)
			display_error_fork(p->r);
		else
			edit_pid_shell(pid, j, p);
		update_status();
		p = p->next;
	}
	act_job(j, fg);
	return (verif);
}
