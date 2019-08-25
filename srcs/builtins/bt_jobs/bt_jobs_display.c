/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/25 14:10:56 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "builtins.h"

char		value_char_job(int current, int max)
{
	if (current == max)
		return ('+');
	if (current == max - 1)
		return ('-');
	return (' ');
}

void		bt_jobs_p(t_job *j, int max_current)
{
	t_process	*p;

	(void)max_current;
	p = j->first_process;
	ft_dprintf(STDOUT_FILENO, "[%d]", j->process_id);
	while (p)
	{
		ft_dprintf(STDOUT_FILENO, " %d", p->pid);
		p = p->next;
	}
	ft_dprintf(STDOUT_FILENO, "\n");
}

static int	bt_jobs_l_display(t_process *p, int last)
{
	char	*cmd;
	int		num_sig;
	char	*name_sig;

	cmd = assembly_cmd_process(p);
	if (last == -1 || last != p->status)
	{
		last = p->status;
		num_sig = (p->status < 32) ? p->status : WSTOPSIG(p->status);
		name_sig = ft_name_sig(num_sig);
		if (num_sig > 0)
			ft_dprintf(STDOUT_FILENO, "\t%d %s: %d\t%s", p->pid, name_sig,
				num_sig, cmd);
		else
			ft_dprintf(STDOUT_FILENO, "\t%d %s\t\t%s", p->pid, name_sig, cmd);
		ft_strdel(&name_sig);
	}
	else
		ft_dprintf(STDOUT_FILENO, "\t%d\t\t\t%s", p->pid, cmd);
	ft_strdel(&cmd);
	return (last);
}

void		bt_jobs_l(t_job *j, int max_current)
{
	t_process	*p;
	char		c;
	int			first_sig;

	p = j->first_process;
	first_sig = -1;
	c = value_char_job(j->current, max_current);
	ft_dprintf(STDOUT_FILENO, "[%d]%c", j->process_id, c);
	while (p)
	{
		first_sig = bt_jobs_l_display(p, first_sig);
		if (p->next)
			ft_dprintf(STDOUT_FILENO, " |\n");
		else
			ft_dprintf(STDOUT_FILENO, "\n");
		p = p->next;
	}
}

int			convert_value_signal(int status)
{
	int	sig;

	sig = status;
	if (sig > 31)
		sig = WSTOPSIG(status);
	if (sig == 0)
		sig = WTERMSIG(status);
	if (sig == 0)
		sig = WEXITSTATUS(status);
	return (sig);
}

int			this_signal(t_process *p)
{
	int		sig;
	int		num_sig;

	sig = convert_value_signal(p->status);
	num_sig = 0;
	while (p)
	{
		num_sig = convert_value_signal(p->status);
		if (sig == 0 || (num_sig != 2 && num_sig != 3 && num_sig != 9
							&& num_sig != 13 && num_sig != 15))
			sig = num_sig;
		p = p->next;
	}
	return (sig);
}

void		bt_jobs_s(t_job *j, int max_current)
{
	t_process	*p;
	char		*str;
	char		*cmd;
	char		c;
	int			num_sig;

	p = j->first_process;
	num_sig = this_signal(p);
	str = ft_inter_signal(num_sig, j);
	if (!str)
		return ;
	cmd = cmd_job_s(j);
	c = value_char_job(j->current, max_current);
	ft_dprintf(STDOUT_FILENO, "[%d]%c\t%s\t%s\n", j->process_id, c, str, cmd);
	ft_strdel(&str);
	ft_strdel(&cmd);
}
