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

static int	exec_pipe(t_job *j, t_process *p, int fg)
{
	int	verif;

	verif = 0;
	if ((verif = is_builtin(j, p, NULL)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		if (p->cmd_path)
			verif = launch_process(p, j->pgid, fg);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	execve("/bin/test", NULL, NULL);
	return (verif);
}

static int	prepare_pipe_not_end(t_process *p, int fd[2], int in)
{
	if (p->r->in == STDIN_FILENO)
		p->r->in = in;
	p->r->fd_pipe = fd[0];
	if (check_is_exec(p->cmd[0], p->r) == 0)
	{
		if (in != 0)
			close(in);
		close(fd[1]);
		return (1);
	}
	return (0);
}

static int	is_not_end(t_job *j, t_process *p, int in, int fg)
{
	int		fd[2];
	pid_t	pid;
	int		verif;

	pipe(fd);
	if (prepare_pipe_not_end(p, fd, in))
		return (fd[0]);
	pid = fork();
	if (pid == 0)
	{
		// dup2(fd[1], STDOUT_FILENO);
		if (p->r->out == STDOUT_FILENO)
			p->r->out = fd[1];
		verif = exec_pipe(j, p, fg);
	}
	else if (pid < 0)
		ft_dprintf(p->r->error, "Error fork\n");
	else
		edit_pid_shell(pid, j, p);  // si edit_pid_path bug sur le pipe mais si enleve bug job control
	close(fd[1]);
	if (in != 0)
		close(in);
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
		verif = exec_pipe(j, p, fg);
	else if (pid < 0)
		ft_dprintf(p->r->error, "Error fork\n");
	else
		edit_pid_shell(pid, j, p);
	if (in != 0)
		close(in);
	return (pid);
}


void
launch_process_test (t_process *p, pid_t pgid,
                int infile, int outfile, int errfile,
                int foreground)
{
  pid_t pid;

  if (get_shell()->interactive)
    {
      /* Put the process into the process group and give the process group
         the terminal, if appropriate.
         This has to be done both by the shell and in the individual
         child processes because of potential race conditions.  */
      pid = getpid ();
      if (pgid == 0) pgid = pid;
      setpgid (pid, pgid);
      if (foreground)
        tcsetpgrp (g_in, pgid);

      /* Set the handling for job control signals back to the default.  */
      signal (SIGINT, SIG_DFL);
      signal (SIGQUIT, SIG_DFL);
      signal (SIGTSTP, SIG_DFL);
      signal (SIGTTIN, SIG_DFL);
      signal (SIGTTOU, SIG_DFL);
      signal (SIGCHLD, SIG_DFL);
    }

  /* Set the standard input/output channels of the new process.  */
  if (infile != STDIN_FILENO)
    {
      dup2 (infile, STDIN_FILENO);
      close (infile);
    }
  if (outfile != STDOUT_FILENO)
    {
      dup2 (outfile, STDOUT_FILENO);
      close (outfile);
    }
  if (errfile != STDERR_FILENO)
    {
      dup2 (errfile, STDERR_FILENO);
      close (errfile);
    }

  /* Exec the new process.  Make sure we exit.  */
  execvp (p->cmd[0], p->cmd);
  perror ("execvp");
  exit (1);
}

void
launch_job_pipe(t_job *j, int foreground)
{
  t_process *p;
  pid_t pid;
  int mypipe[2], infile, outfile;

  infile = g_in;
  for (p = j->first_process; p; p = p->next)
    {
      /* Set up pipes, if necessary.  */
      if (p->next)
        {
          if (pipe (mypipe) < 0)
            {
              perror ("pipe");
              exit (1);
            }
          outfile = mypipe[1];
        }
      else
        outfile = p->r->out;

      /* Fork the child processes.  */
      pid = fork ();
      if (pid == 0)
        /* This is the child process.  */
        launch_process_test(p, j->pgid, infile,
                        outfile, p->r->error, foreground);
      else if (pid < 0)
        {
          /* The fork failed.  */
          perror ("fork");
          exit (1);
        }
      else
        {
          /* This is the parent process.  */
          p->pid = pid;
          if (get_shell()->interactive)
            {
              if (!j->pgid)
                j->pgid = pid;
              setpgid (pid, j->pgid);
            }
        }

      /* Clean up after pipes.  */
      if (infile != p->r->in)
        close (infile);
      if (outfile != p->r->out)
        close (outfile);
      infile = mypipe[0];
    }

  job_info (j, "launched");

  if (!get_shell()->interactive)
    wait_for_job(j);
  else if (foreground)
    add_in_fg(j, 0);
  else
    add_in_bg(j, 0);
}




// void	launch_process_test(t_process *p, pid_t pgid, int in,
// 			int out, int err, int fg)
// {
// 	pid_t	pid;
// 	t_shell	*s;
// 	char	**environ;
	
// 	environ = create_list_env(get_env(0, NULL), 1);
// 	s = get_shell();
// 	if (s->interactive)
// 	{
// 		pid = getpid();
// 		if (pgid == 0)
// 			pgid = pid;
// 		setpgid (pid, pgid);
// 		if (fg)
// 			tcsetpgrp(s->term, pgid);
// 		dfl_signaux();
// 	}
// 	if (p->r->fd_pipe != -1)
// 		close(p->r->fd_pipe);
// 	if (in != STDIN_FILENO)
// 	{
// 		dup2(in, STDIN_FILENO);
// 		close(in);
// 	}
// 	if (out != STDOUT_FILENO)
// 	{
// 		dup2(out, STDOUT_FILENO);
// 		close(out);
// 	}
// 	if (err != STDERR_FILENO)
// 	{
// 		dup2(err, STDERR_FILENO);
// 		close(err);
// 	}
// 	execve(p->cmd_path, p->cmd, environ);
// 	ft_dprintf(p->r->error, "21sh: command not found: %s\n", p->cmd[0]);
// 	execve("/bin/test", NULL, NULL);
// 	exit(1);
// }



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
