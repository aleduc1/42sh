/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
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

static void	launch_process_pipe(t_process *p)
{
	char	**environ;

	environ = create_list_env(get_env(0, NULL), 0);
	execve(p->cmd_path, p->cmd, environ);
	display_error_command(p->r, p->cmd);
	exit(1);
}

static int	exec_pipe(t_job *j, t_process *p, int fg)
{
	int	verif;

	verif = 0;
	config_pid_process(j->pgid, fg);
	if (p->return_value == 1)
	{
		execve("/bin/test", NULL, NULL);
		exit(1);
	}
	if ((verif = is_builtin(j, p, NULL)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		if (p->cmd_path)
		{
			redirection_fd(p->r);
			launch_process_pipe(p);
		}
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	execve("/bin/test", NULL, NULL);
	exit(verif);
}

static void	fork_pipe(t_job *j, t_process *p, int fg, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (fd[0] != p->r->in && fd[0] != STDIN_FILENO && verif_close(fd[0]))
			close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (verif_close(fd[1]))
			close(fd[1]);
		exec_pipe(j, p, fg);
	}
	else if (pid < 0)
	{
		ft_dprintf(j->r->error, "42sh: error fork\n");
		exit(1);
	}
	else
		edit_pid_shell(pid, j, p);
}

static void	pipe_error(int error)
{
	ft_dprintf(error, "42sh: error pipe\n");
	bt_exit(NULL, NULL);
}

/*
** LINE 221: fd[0] = j->r->in;//open(STDIN_FILENO, O_RDWR);// j->r->in;
*/

void		launch_job_pipe(t_job *j, int fg)
{
	t_process	*p;
	int			fd[2];

	p = j->first_process;
	fd[0] = STDIN_FILENO;
	while (p)
	{
		if (p->next)
			if (pipe(fd) == -1)
				pipe_error(j->r->error);
		fork_pipe(j, p, fg, fd);
		if (p->r->in != j->r->in && verif_close(p->r->in))
			close(p->r->in);
		if (fd[1] != j->r->out && verif_close(fd[1]))
			close(fd[1]);
		p = p->next;
		if (p)
			pipe_deplace_fd(j, p, fd[0]);
	}
	act_job(j, fg);
}
