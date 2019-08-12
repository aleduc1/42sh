/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/05 02:17:05 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	display_command_not_found(t_redirection *r, char *cmd)
{
	ft_dprintf(r->error, "42sh: %s: command not found\n", cmd);
}

void	display_permission_denied(t_redirection *r, char *cmd)
{
	ft_dprintf(r->error, "42sh: %s: Permission denied\n", cmd);
}

void	display_error_fork(t_redirection *r)
{
	ft_dprintf(r->error, "42sh: Error fork\n");
}

void	display_too_many_arg(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: Too many arguments.\n", name);
}

void	display_too_few_arg(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: Too few arguments.\n", name);
}

void	display_nothing_value(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: You are nothing value\n", name);
}

void	display_bad_file_descriptor(int error)
{
	ft_dprintf(error, "42sh: Bad file descriptor\n");
}

void	display_no_such_job(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: jobs %s: no such job\n", name);
}

void	display_invalid_option_job(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: jobs %s: invalid option\n", name);
	ft_dprintf(r->error, "jobs: usage: jobs [-l | -p] [job_id...]\n");
}

void	display_no_current_job(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: no current job\n", name);
}

void	display_no_job_control(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: no job control\n", name);
}

void	display_job_stopped(void)
{
	ft_dprintf(STDERR_FILENO, "42sh: There are stopped jobs.\n");
}

void	display_kill_not_work(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: %s: Kill not work!\n", name);
}

void	display_error_tc(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "42sh: error: %s\n", name);
}

void	display_other_error(int fd_error, char *name, char *error)
{
	ft_dprintf(fd_error, "42sh: %s: %s\n", name, error);
}

void	display_error_expansion(char *src)
{
	ft_dprintf(STDERR_FILENO, "42sh: ${%s}: bad substitution\n", src);
	gest_return(-5);
}
