/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 09:42:05 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:28:20 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

int		launch_process(t_process *p, pid_t pgid, int fg)
{
	pid_t	pid;
	char	**environ;
	t_shell	*s;
	int		verif;

	s = get_shell();
	dfl_signaux();
	environ = create_list_env(get_env(0, NULL), 1);
	if (s->interactive)
	{
		pid = getpid();
		if (pgid == 0)
			pgid = pid;
		setpgid(pid, pgid);
		if (fg)
			tcsetpgrp(s->term, pgid);
	}
	redirection_fd(p->r);
	verif = execve(p->cmd_path, p->cmd, environ);
	ft_dprintf(p->r->error, "21sh: command not found: %s\n", p->cmd[0]);
	execve("/bin/test", NULL, NULL);
	exit(verif);
}

void	act_job(t_job *j, int fg)
{
	t_shell	*shell;

	shell = get_shell();
	if (!shell->interactive)
		wait_for_jobs(j);
	else if (fg)
		add_in_fg(j, 0);
	else
	{
		job_info(j, "launched");
		add_in_bg(j, 0);
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
			verif = launch_process(p, j->pgid, fg);
		else if (pid < 0)
			ft_dprintf(p->r->error, "Error fork\n");
		else
		{
			p->pid = pid;
			if (get_shell()->interactive)
			{
				if (!(j->pgid))
					j->pgid = pid;
				setpgid(pid, j->pgid);
			}
		}
		update_status();
		p = p->next;
	}
	act_job(j, fg);
	return (verif);
}

static int	is_not_end(t_job *j, t_process *p, int in, int fg)
{
	int		fd[2];
	pid_t	pid;
	int		verif;

	pipe(fd);
	if (p->r->in == STDIN_FILENO)
		p->r->in = in;
	p->r->fd_pipe = fd[0];
	if (check_is_exec(p->cmd[0], p->r) == 0)
	{
		if (in != 0)
			close(in);
		close(fd[1]);
		return (fd[0]);
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
	if (p->r->out == STDOUT_FILENO)
		p->r->out = fd[1];
		if ((verif = is_builtin(j, p, NULL)) == -1)
		{
			p->cmd_path = is_in_path(p->cmd[0]);
			verif = (p->cmd_path) ? launch_process(p, j->pgid, fg) : gest_error_path(p->cmd[0], p->r);
		}
		execve("/bin/test", NULL, NULL);
	}
	else if (pid < 0)
		ft_dprintf(p->r->error, "Error fork\n");
	else
	{
		p->pid = pid;
		if (get_shell()->interactive)
		{
			if (!(j->pgid))
				j->pgid = pid;
			setpgid(pid, j->pgid);
		}
	}
	if (in != 0)
		close(in);
	close(fd[1]);
	return (fd[0]);
}

static int	is_end(t_job *j, t_process *p, int in, int fg)
{
	pid_t	pid;
	int		verif;

	if (p->r->in == STDIN_FILENO)
		p->r->in = in;
	p->r->fd_pipe = -1;
	if (check_is_exec(p->cmd[0], p->r) == 0)
	{
		close(in);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		if ((verif = is_builtin(j, p, NULL)) == -1)
		{
			p->cmd_path = is_in_path(p->cmd[0]);
			verif = (p->cmd_path) ? launch_process(p, j->pgid, fg) : gest_error_path(p->cmd[0], p->r);
		}
		execve("/bin/test", NULL, NULL);
	}
	else if (pid < 0)
		ft_dprintf(p->r->error, "Error fork\n");
	else
	{
		p->pid = pid;
		if (get_shell()->interactive)
		{
			if (!(j->pgid))
				j->pgid = pid;
			setpgid(pid, j->pgid);
		}
	}
	close(in);
	return (pid);
}

int			launch_job_pipe(t_job *j, int fg)
{
	t_process	*p;
	int			in;
	int			ret;

	in = 0;
	ret = 0;
	p = j->first_process;
	while (p)
	{
		if (p->next)
			in = is_not_end(j, p, in, fg);
		else
			in = is_end(j, p, in, fg);
		p = p->next;
	}
	act_job(j, fg);
	update_status();
	return (0);
}
