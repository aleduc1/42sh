/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hab <hab@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/22 09:20:19 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "job.h"

/*
** search if it's a builtin:
**	return 0 if it's a builtin and if it's command work
**	return -2 if it's a builtin and if it's command not work
**	return -1 if it's not a builtin
*/

/*
** Check if builtin contains the right number of args
** Args:	char **argv -> command
**			int nb -> nb of allowed args
**			char *name -> name of builtin
*/

static int	verif_set(char **argv, int nb, t_redirection *r, char *name)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	if (nb == i || (nb == 3 && i == 2))
		return (1);
	else if (i > nb)
		display_too_many_arg(r, name);
	else
	{
		if (ft_strequ(name, "setenv"))
			builtin_env_display(r);
		else
			display_too_few_arg(r, name);
	}
	return (0);
}

/*
**	setenv -> edit_setenv(av[1], av[2]);
**	unsetenv -> ft_unsetenv(av);
** Args:	
*/

static int	is_builtin_env(t_process *p, char **av, t_pos *pos)
{
	int	verif;

	if (ft_strequ(av[0], "set"))
		verif = verif_set(p->cmd, 1, p->r, "set") ? builtin_set(p->r) : 1;
	else if (ft_strequ(av[0], "export"))
		verif = bt_export(av, p->r);
	else if (ft_strequ(av[0], "unset"))
		verif = ft_unset(av);
	else if (ft_strchr_exist(av[0], '='))
		verif = edit_set(av, p->r, pos);
	else
		verif = -1;
	return (verif);
}

static int	is_builtin_jobs(t_job *j, char **av, t_redirection *r)
{
	int	verif;

	verif = -1;
	if (ft_strequ(av[0], "jobs"))
		verif = bt_jobs(j, av, r);
	else if (ft_strequ(av[0], "fg"))
		verif = bt_fg(j, av, r);
	else if (ft_strequ(av[0], "bg"))
		verif = bt_bg(j, av, r);
	return (verif);
}

static int	is_builtin_other(t_pos *pos, char **av, t_redirection *r)
{
	int	verif;

	verif = -1;
	if (ft_strequ(av[0], "fc"))
	{
		redirection_fd(r);
		verif = builtin_fc(av, pos);
	}
	else if (ft_strequ(av[0], "test"))
		verif = bt_test(av, r);
	else if (ft_strequ(av[0], "alias"))
		verif = bt_alias(av, r);
	else if (ft_strequ(av[0], "unalias"))
		verif = bt_unalias(av, r);
	else if (ft_strequ(av[0], "hash"))
		verif = bt_hash(av, r);
	return (verif);
}

void		restore_redirection(void)
{
	char	*fd_s;
	int		fd;

	fd_s = value_line_path("STDIN", 0);
	fd = ft_atoi(fd_s);
	ft_strdel(&fd_s);
	dup2(fd, STDIN_FILENO);
	fd_s = value_line_path("STDOUT", 0);
	fd = ft_atoi(fd_s);
	ft_strdel(&fd_s);
	dup2(fd, STDOUT_FILENO);
	fd_s = value_line_path("STDERR", 0);
	fd = ft_atoi(fd_s);
	ft_strdel(&fd_s);
	dup2(fd, STDERR_FILENO);
}

/*
** if builtin not exist return -1
** if exist and good execution return 0 otherwise return -2
*/

int			is_builtin(t_job *j, t_process *p, t_pos *pos)
{
	int		verif;
	char	**av;

	av = p->cmd;
	verif = is_builtin_env(p, av, pos);
	if (verif != -1)
		;
	else if (ft_strequ(av[0], "echo"))
		verif = bt_echo(av, p->r);
	else if (ft_strequ(av[0], "cd"))
		verif = bt_cd(av, p->r);
	else if (ft_strequ(av[0], "exit"))
		verif = bt_exit(j, pos, p->r);
	else if (ft_strequ(av[0], "type"))
		verif = bt_type(av, p->r);
	else if ((verif = is_builtin_jobs(j, av, p->r)) != -1)
		;
	else if ((verif = is_builtin_other(pos, av, p->r)) != -1)
		;
	else
		verif = -1;
	if (verif != -1)
	{
		restore_redirection();
		gest_return(verif == -2 ? 1 : verif);
	}
	return (verif);
}

int			builtin_exist(char *cmd)
{
	if (ft_strequ(cmd, "echo") || ft_strequ(cmd, "cd") || ft_strequ(cmd, "exit")
		|| ft_strequ(cmd, "fc") || ft_strequ(cmd, "test")
		|| ft_strequ(cmd, "alias") || ft_strequ(cmd, "unalias")
		|| ft_strequ(cmd, "hash") || ft_strequ(cmd, "type")
		|| ft_strequ(cmd, "jobs")
		|| ft_strequ(cmd, "fg") || ft_strequ(cmd, "bg")
		|| ft_strequ(cmd, "set")
		|| ft_strequ(cmd, "export")
		|| ft_strequ(cmd, "unset") || ft_strchr_exist(cmd, '='))
		return (1);
	return (0);
}

/*
** Folowing of launch_job_blt function
*/

int			launch_process_blt(t_job *j, t_process *p, t_pos *pos,
				int fg)
{
	char	**environ;
	int		verif;

	environ = create_list_env(get_env(0, NULL), 1);
	config_pid_process(j->pgid, fg);
	redirection_fd(p->r);
	verif = is_builtin(j, p, pos);
	execve_bin_test();
	exit(verif);
}

/*
** launch builtin in fork if is background command
*/

int			launch_job_blt(t_job *j, t_process *p, t_pos *pos, int fg)
{
	pid_t		pid;
	int			verif;

	verif = 0;
	pid = fork();
	if (pid == 0)
		verif = launch_process_blt(j, p, pos, fg);
	else if (pid < 0)
		display_error_fork(p->r);
	else
		edit_pid_shell(pid, j, p);
	update_status();
	p = p->next;
	act_job(j, fg);
	return (verif);
}

/*
** Launch builtin command
*/

int			builtin(t_job *j, t_process *p, t_pos *pos, int fg)
{
	int		verif;

	verif = -1;
	if (fg == 1)
		return (is_builtin(j, p, pos));
	else
		launch_job_blt(j, p, pos, fg);
	return (verif);
}
