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
		if (setpgid(pid, pgid) == -1)
			exit(1);
		if (fg)
			tcsetpgrp(s->term, pgid);
	}
	redirection_fd(p->r);
	verif = execve(p->cmd_path, p->cmd, environ);
	display_error_command(p->r, p->cmd);
	execve("/bin/test", NULL, NULL);
	exit(verif);
}

void		act_job(t_job *j, int fg)
{
	t_shell	*shell;

	shell = get_shell();
	if (!shell->interactive)
		wait_for_job(j);
	else if (fg)
		add_in_fg(j, 0);
	else
	{
		job_info(j, "launched");
		add_in_bg(j, 0);
	}
}

static void	edit_pid_shell(int pid, t_job *j, t_process *p)
{
	p->pid = pid;
	if (get_shell()->interactive)
	{
		if (!(j->pgid))
			j->pgid = pid;
		if (setpgid(pid, j->pgid) == -1)
			exit(1);
	}
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
			verif = launch_process(p, j->pgid, fg);
		else if (pid < 0)
			ft_dprintf(p->r->error, "42sh: Error fork\n");
		else
			edit_pid_shell(pid, j, p);
		update_status();
		p = p->next;
	}
	act_job(j, fg);
	return (verif);
}

void		config_pid_process(pid_t pid, pid_t pgid, int fg)
{
	t_shell	*s;

	s = get_shell();
	if (s->interactive)
	{
		pid = getpid();
		if (pgid == 0)
			pgid = pid;
		if (setpgid(pid, pgid) == -1)
			exit(1);
		if (fg)
			tcsetpgrp(s->term, pgid);
		dfl_signaux();
	}
}

void		launch_process_test(t_process *p)
{
	char	**environ;

	environ = create_list_env(get_env(0, NULL), 0);
	execve(p->cmd_path, p->cmd, environ);
	display_error_command(p->r, p->cmd);
	exit(1);
}

static int	exec_pipe(t_job *j, t_process *p, int fg, pid_t pid)
{
	int	verif;

	verif = 0;
	config_pid_process(pid, j->pgid, fg);
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
			launch_process_test(p);
		}
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	execve("/bin/test", NULL, NULL);
	exit(verif);
}

void		fork_pipe(t_job *j, t_process *p, int fg, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (fd[0] != p->r->in && fd[0] != STDIN_FILENO)
			close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exec_pipe(j, p, fg, pid);
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

int			edit_fd(int base, int origin, int new)
{
	if (base == origin)
		return (new);
	close(new);
	return (base);
}

void		pipe_deplace_fd(t_job *j, t_process *p, int fd)
{
	if (p->r->in == j->r->in)
		p->r->in = fd;
	else
	{
		if (fd != j->r->in)
			close(fd);
	}
}

void		launch_job_pipe(t_job *j, int fg)
{
	t_process	*p;
	int			fd[2];

	p = j->first_process;
	fd[0] = STDIN_FILENO;//j->r->in;//open(STDIN_FILENO, O_RDWR);// j->r->in;
	while (p)
	{
		if (p->next)
			if (pipe(fd) == -1)
				pipe_error(j->r->error);
		fork_pipe(j, p, fg, fd);
		if (p->r->in != j->r->in)
			close(p->r->in);
		if (fd[1] != j->r->out)
			close(fd[1]);
		p = p->next;
		if (p)
			pipe_deplace_fd(j, p, fd[0]);
	}
	act_job(j, fg);
}
