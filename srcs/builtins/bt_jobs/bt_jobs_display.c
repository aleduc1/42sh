/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/17 22:59:10 by sbelondr         ###   ########.fr       */
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

void		bt_jobs_p(t_job *j, int max_current, t_redirection *r)
{
	t_process	*p;

	(void)max_current;
	p = j->first_process;
	ft_dprintf(r->out, "[%d]", j->process_id);
	while (p)
	{
		ft_dprintf(r->out, " %d", p->pid);
		p = p->next;
	}
	ft_dprintf(r->out, "\n");
}

static int	bt_jobs_l_display(t_process *p, t_redirection *r, int last)
{
	char	*cmd;
	int		num_sig;
	char	*name_sig;

	cmd = assembly_cmd_process(p);
	if (last == -1 || last != p->status)
	{
		if (last == -1)
			last = p->status;
		num_sig = (p->status < 32) ? p->status : WSTOPSIG(p->status);
		name_sig = ft_name_sig(num_sig);
		if (num_sig > 0)
			ft_dprintf(r->out, "\t%d %s: %d\t%s", p->pid, name_sig,
				num_sig, cmd);
		else
			ft_dprintf(r->out, "\t%d %s\t%s", p->pid, name_sig, cmd);
		ft_strdel(&name_sig);
	}
	else
		ft_dprintf(r->out, "\t%d\t\t\t%s", p->pid, cmd);
	ft_strdel(&cmd);
	return (last);
}

void		bt_jobs_l(t_job *j, int max_current, t_redirection *r)
{
	t_process	*p;
	char		c;
	int			first_sig;

	p = j->first_process;
	first_sig = -1;
	c = value_char_job(j->current, max_current);
	ft_dprintf(r->out, "[%d]%c", j->process_id, c);
	while (p)
	{
		first_sig = bt_jobs_l_display(p, r, first_sig);
		if (p->next)
			ft_dprintf(r->out, " |\n");
		else
			ft_dprintf(r->out, "\n");
		p = p->next;
	}
}

void		bt_jobs_s(t_job *j, int max_current, t_redirection *r)
{
	t_process	*p;
	char		*str;
	char		*cmd;
	char		c;
	int			num_sig;

	p = j->first_process;
	while (p)
	{
		if (!p->next)
			break ;
		p = p->next;
	}
	num_sig = (p->status < 32) ? p->status : WSTOPSIG(p->status);
	str = ft_inter_signal(num_sig, j);
	if (!str)
		return ;
	cmd = cmd_job_s(j);
	c = value_char_job(j->current, max_current);
	ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->process_id, c, str, cmd);
	ft_strdel(&str);
	ft_strdel(&cmd);
}
