/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/24 06:16:49 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"

void	execve_bin_test(void)
{
	char	**test;

	if (!(test = (char**)malloc(sizeof(char*) * 2)))
		return ;
	test[0] = ft_strdup("test");
	test[1] = 0;
	execve("/bin/test", test, NULL);
	ft_arraydel(&test);
}

char	*name_exec_error(void)
{
	char	*name_exec;

	name_exec = ft_strjoin(get_shell()->name_shell, ": ");
	return (name_exec);
}

void	display_command_not_found(t_redirection *r, char *cmd)
{
	char	*name_exec;
	char	*display;
	char	*cache;

	name_exec = name_exec_error();
	cache = ft_strjoin(name_exec, cmd);
	ft_strdel(&name_exec);
	display = ft_strjoin(cache, ": command not found\n");
	ft_putstr_fd(display, r->error);
	ft_strdel(&display);
	ft_strdel(&cache);
}

void	display_permission_denied(t_redirection *r, char *cmd)
{
	char	*name_exec;
	char	*display;
	char	*cache;

	name_exec = name_exec_error();
	cache = ft_strjoin(name_exec, cmd);
	ft_strdel(&name_exec);
	display = ft_strjoin(cache, ": Permission denied\n");
	ft_putstr_fd(display, r->error);
	ft_strdel(&display);
	ft_strdel(&cache);
}

void	display_error_fork(t_redirection *r)
{
	ft_dprintf(r->error, "%s: Error fork\n", get_shell()->name_shell);
}

void	display_too_many_arg(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: %s: Too many arguments.\n",
			get_shell()->name_shell, name);
}

void	display_too_few_arg(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: %s: Too few arguments.\n",
			get_shell()->name_shell, name);
}

void	display_nothing_value(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: %s: You are nothing value\n",
			get_shell()->name_shell, name);
}

void	display_bad_file_descriptor(int error)
{
	ft_dprintf(error, "%s: Bad file descriptor\n", get_shell()->name_shell);
}

void	display_no_such_job(t_redirection *r, char *name_blt, char *name)
{
	ft_dprintf(r->error, "%s: %s: %s: no such job\n", get_shell()->name_shell,
		name_blt, name);
}

void	display_invalid_option_job(t_redirection *r, char *name)
{
	char	c;

	c = 0;
	while (*name)
	{
		if (*name != 'p' && *name != 'l')
		{
			c = *name;
			break ;
		}
		++name;
	}
	ft_dprintf(r->error, "%s: jobs -%c: invalid option\n",
			get_shell()->name_shell, c);
	ft_dprintf(r->error, "jobs: usage: jobs [-l | -p] [job_id...]\n");
}

void	display_no_current_job(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: %s: no current job\n",
			get_shell()->name_shell, name);
}

void	display_no_job_control(t_redirection *r, char *name)
{
	char	*name_shell;
	char	*data;
	char	*cache;

	name_shell = name_exec_error();
	cache = ft_strjoin(name_shell, name);
	ft_strdel(&name);
	data = ft_strjoin(cache, ": no job control\n");
	ft_putstr_fd(data, r->error);
	ft_strdel(&cache);
	ft_strdel(&data);
}

void	display_job_stopped(void)
{
	ft_dprintf(STDERR_FILENO, "%s: There are stopped jobs.\n",
			get_shell()->name_shell);
}

void	display_kill_not_work(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: %s: Kill not work!\n",
			get_shell()->name_shell, name);
}

void	display_error_tc(t_redirection *r, char *name)
{
	ft_dprintf(r->error, "%s: error: %s\n", get_shell()->name_shell, name);
}

void	display_other_error(int fd_error, char *name, char *error)
{
	ft_dprintf(fd_error, "%s: %s: %s\n", get_shell()->name_shell, name, error);
}

void	display_error_expansion(char *src)
{
	ft_dprintf(STDERR_FILENO, "%s: ${%s}: bad substitution\n",
			get_shell()->name_shell, src);
	gest_return(-5);
}

void	display_error_syntax(char *key, char *value)
{
	ft_dprintf(STDERR_FILENO, "%s: %s: %s: syntax error: operand expected\n",
			get_shell()->name_shell, key, value);
	gest_return(-5);
}
