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

#include "sh21.h"
#include "job.h"

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
		setpgid(pid, j->pgid);
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
			edit_pid_shell(pid, j, p);
		update_status();
		p = p->next;
	}
	act_job(j, fg);
	return (verif);
}


static void	dup_pipe(int fd[2], int in, t_redirection *r)
{
	if (in != STDIN_FILENO)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (fd[1] != STDOUT_FILENO)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (fd[0] > 2)
		close(fd[0]);
	if (r->in == STDIN_FILENO)
		r->in = in;
	if (r->out == STDOUT_FILENO)
		r->out = fd[1];
}

void	launch_process_test (t_process *p, pid_t pgid, int fg)
{
	pid_t	pid;
	char	**environ;
	t_shell	*s;

	s = get_shell();	
	environ = create_list_env(get_env(0, NULL), 0);
	if (s->interactive)
	{
		pid = getpid();
		if (pgid == 0)
			pgid = pid;
		setpgid(pid, pgid);
		if (fg)
			tcsetpgrp(s->term, pgid);
		dfl_signaux();
	}
	redirection_fd(p->r);
	execve(p->cmd_path, p->cmd, environ);
	perror ("execve");
	exit (1);
}

static int	exec_pipe(t_job *j, t_process *p, int fg)
{
	int	verif;

	verif = 0;
	if ((verif = is_builtin(j, p, NULL)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		if (p->cmd_path)
			launch_process_test(p, j->pgid, fg);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	execve("/bin/test", NULL, NULL);
	return (verif);
}

void	is_not_end(t_job *j, t_process *p, int in, int fd[2], int fg)
{
	pid_t	pid;

	pid = fork ();
	if (pid == 0)
	{
		dup_pipe(fd, in, p->r);
		exec_pipe(j, p, fg);
	}
	else if (pid < 0)
	{
		ft_dprintf(STDOUT_FILENO, "42sh: error fork\n");
		exit (1);
	}
	else
	{
		if (fd[1] > 2)
			close(fd[1]);
		if (in != STDIN_FILENO)
			close(in);
		p->pid = pid;
		if (get_shell()->interactive)
		{
			if (!j->pgid)
				j->pgid = pid;
			setpgid(pid, j->pgid);
		}
	}
}

void	is_end(t_job *j, t_process *p, int fd[2], int fg)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		exec_pipe(j, p, fg);
	else if (pid < 0)
	{
		ft_dprintf(STDOUT_FILENO, "42sh: error fork\n");
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (fd[0] > 2)
			close(fd[0]);
		// while ((pid = wait(&status)) > 0)
		// 	j->pgid = pid;
		p->pid = pid;
		if (get_shell()->interactive)
		{
			if (!j->pgid)
				j->pgid = pid;
			setpgid(pid, j->pgid);
		}
	}
}


void	launch_job_pipe(t_job *j, int fg)
{
	t_process	*p;
	int			fd[2];
	int			in;

	in = STDIN_FILENO;
	p = j->first_process;
	dfl_signaux();
	while (p)
	{
		if (p->next)
		{
			if (pipe(fd) == -1)
			{
				perror ("pipe");
				exit (1);
			}
			is_not_end(j, p, in, fd, fg);
			in = fd[0];
		}
		else
		{
			if (p->r->in == STDIN_FILENO)
				p->r->in = in;
			is_end(j, p, fd, fg);
		}
		p = p->next;
	}
	act_job(j, fg);
	ign_signaux();
}


// void		launch_job_pipe(t_job *j, int fg)
// {
// 	t_process		*p;
// 	int				in;
// 	int				out;
// 	t_redirection	*r;
// 	pid_t			pid;
// 	int				fd[2];

// 	r = init_redirection();
// 	p = j->first_process;
// 	in = r->in;
// 	while (p)
// 	{
// 		if (p->next)
// 		{
// 			if (pipe(fd) < 0)
// 				exit (1);
// 			out = fd[1];
// 		}
// 		else
// 			out = p->r->out;
// 		ft_printf("in = %d, out = %d, fd[0] = %d, fd[1] = %d, r->in = %d, r->out = %d\n",
// 			in, out, fd[0], fd[1], r->in, r->out);
// 		pid = fork();
// 		if (pid == 0)
// 		{

// 			p->cmd_path = is_in_path(p->cmd[0]);
// 			if (p->cmd_path)
// 				launch_process_test(p, j->pgid, in, out, p->r->error, fg);
// 			else
// 				gest_error_path(p->cmd[0], p->r);
// 		}
// 		else if (pid < 0)
// 			ft_dprintf(p->r->error, "Error fork\n");
// 		else
// 		{
// 			p->pid = pid;
// 			if (get_shell()->interactive)
// 			{
// 				if (!j->pgid)
// 					j->pgid = pid;
// 				setpgid (pid, j->pgid);
// 			}
// 		}
// 		if (in != r->in)
// 			ft_printf("in %d\n", close(in));
// 		if (out != r->out)
// 			ft_printf("out %d\n", close(out));
// 		in = fd[0];
// 		p = p->next;
// 	}
// 	act_job(j, fg);
// 	update_status();
// 	delete_redirection(&r);
// }
