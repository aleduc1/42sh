/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 13:52:06 by apruvost          #+#    #+#             */
/*   Updated: 2019/05/27 10:48:03 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"
#include "builtins.h"

extern t_ht_hash	*g_alias_table;
extern t_ht_hash	*g_hash_table;

/*
** exit [n]
**
** exit utility shall cause shell to exit from current execution env with exit
** status specified by unsigned decimal int n
** If current execution env is subshell env,
**    shell shall exit from subshell env with specified exit status and
** continue in env from which that subshell was invoked
** otherwise, shell utility shall terminate with specified exit status
** If n is specified, but its value is not etween 0 and 255 inclusively, the
** exit status is undefined
**
** A trap on EXIT shall be executed before shell terminates, except when exit
** utility invoked in trap itself, which case, shell exit immediately
**
** EXIT STATUS
**
** exit status shall be n, if specified, except thaht eaviour unspecified if
** not unsigned decimal int or greater than 255
** Otherwise, value shall be exit value of last command executed, or zero if no
** command executed
** When exit executed in trap action, last command is considered ti eb command
** that executed immediately preceding trap action
**
** RATIONALE
**
** certain exit status have een reserved for special uses and should be used by
** application only for those purposes :
** 126
**     A file to be executed was found, but it was not an executable utility
** 127
**     A utility to be executed was not found
** >128
**     A command was interrupted by a signal
**
** Behaviour of exit when given invalid argument or unknown option is
** unspecified, because of different practices in various historical implement
** A value larger than 255 might be truncated by shell, and be unavailable even
** to parent process taht uses waitid() to get full exit value
** It is recommanded thaht implementations that detect any usage error should
** cause a non-zero exit status
** (or if shell is interactive and error does not cause shell to abort, store
** non-zero value in "$?") but even this not done historicaly in all shells
*/

/*
** Remplacer tous les g_status par la variable $? des variables locales ou la
** valeur de retour du shell
*/

static void	close_std(void)
{
	char	*tmp;
	int		std;

	tmp = value_line_path("STDIN", 0);
	if (tmp)
	{
		std = ft_atoi(tmp);
		close(std);
		ft_strdel(&tmp);
	}
	tmp = value_line_path("STDOUT", 0);
	if (tmp)
	{
		std = ft_atoi(tmp);
		close(std);
		ft_strdel(&tmp);
	}
	tmp = value_line_path("STDERR", 0);
	if (tmp)
	{
		std = ft_atoi(tmp);
		close(std);
		ft_strdel(&tmp);
	}
}

static void	exec_reset_shell(t_pos *pos)
{
	if (pos)
		history_file(pos->history);
	close_std();
	free_all_job();
	default_term_mode();
	delete_shell();
	get_env(1, NULL);
	ht_hash_del(g_alias_table);
	ht_hash_del(g_hash_table);
}

int			bt_exit(t_job *j, t_pos *pos)
{
	int	rt;

	if ((!j) || (!j->first_process->cmd) || (!j->first_process->cmd[1]))
	{
		exec_reset_shell(pos);
		ft_dprintf(2, "exit\n");
		exit(0);
	}
	if (ft_isstrnum(j->first_process->cmd[1]))
	{
		if (!j->first_process->cmd[2])
		{
			rt = ft_atoi(j->first_process->cmd[1]);
			exec_reset_shell(pos);
			ft_dprintf(2, "exit\n");
			exit(rt);
		}
		ft_dprintf(2, "42sh: exit: too many arguments\n");
		return (1);
	}
	ft_dprintf(2, "42sh: exit: %s: numeric argument required\n",
			j->first_process->cmd[1]);
	exec_reset_shell(pos);
	exit(255);
}
