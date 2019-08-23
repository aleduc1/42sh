/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/24 01:20:44 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"

char		*assembly_cmd_process(t_process *p)
{
	char	*str;
	char	*tmp;
	char	*cache;
	int		i;

	i = -1;
	str = ft_strdup(p->cmd[++i]);
	while (p->cmd[++i])
	{
		tmp = ft_strjoin(str, " ");
		cache = ft_strjoin(tmp, p->cmd[i]);
		ft_strdel(&str);
		ft_strdel(&tmp);
		str = cache;
	}
	return (str);
}

static void	assembly_command_s(t_process *p, char **str)
{
	char		*tmp;
	char		*cache;

	if (!(*str))
		(*str) = assembly_cmd_process(p);
	else
	{
		tmp = assembly_cmd_process(p);
		cache = ft_strjoin(*str, tmp);
		ft_strdel(str);
		ft_strdel(&tmp);
		(*str) = cache;
	}
	if (p->next)
	{
		cache = ft_strjoin(*str, " | ");
		ft_strdel(str);
		(*str) = cache;
	}
}

char		*cmd_job_s(t_job *j)
{
	t_process	*p;
	char		*str;
	char		*cache;

	p = j->first_process;
	str = NULL;
	while (p)
	{
		assembly_command_s(p, &str);
		p = p->next;
	}
	if (j->fg == 0)
	{
		cache = ft_strjoin(str, " &");
		ft_strdel(&str);
		str = cache;
	}
	return (str);
}

/*
** WTERMSIG(j->first_process->status));
** LINE 47:
** else
** 	str = ft_itoa(sig);
*/

char		*ft_inter_signal(int sig, t_job *j)
{
	char	*str;

	str = NULL;
	if (sig == 0 && job_is_completed(j) && j->fg == 1)
		str = ft_strdup("Terminated: 15");
	else if (sig == 0 && job_is_completed(j) && j->fg == 0)
		str = ft_strdup("Done\t\t");
	else if (sig == 0 && (!job_is_completed(j)) && (!job_is_stopped(j)))
		str = ft_strdup("Running\t\t");
	else if (sig == 1)
		str = ft_strdup("Done(SIGHUP)");
	else if (sig == 2)
		str = ft_strdup("Terminated(SIGINT)");
	else if (sig == 3)
		str = ft_strdup("Terminated(SIGQUIT)");
	else if (sig == 9)
		str = ft_strdup("Terminated(SIGKILL)");
	else if (sig == 13)
		str = ft_strdup("Terminated(SIGPIPE)");
	else if (sig == 15)
		str = ft_strdup("Terminated(SIGTERM)");
	else if (sig == 17)
		str = ft_strdup("Stopped(SIGSTOP)");
	else if (sig == 18)
		str = ft_strdup("Stopped(SIGTSTP)");
	else if (sig == 19)
		str = ft_strdup("Running(SIGCONT)");
	else if (sig == 21)
		str = ft_strdup("Stopped(SIGTTIN)");
	else if (sig == 22)
		str = ft_strdup("Stopped(SIGTTOU)");
	else
		str = ft_itoa(sig);
	return (str);
}
