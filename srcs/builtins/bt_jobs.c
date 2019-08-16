/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 10:54:45 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/15 22:27:00 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

char		*assemply_cmd_process(t_process *p)
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

char		*cmd_job_s(t_job *j)
{
	t_process	*p;
	char		*str;
	char		*tmp;
	char		*cache;

	p = j->first_process;
	str = NULL;
	while (p)
	{
		if (!str)
			str = assemply_cmd_process(p);
		else
		{
			tmp = assemply_cmd_process(p);
			cache = ft_strjoin(str, tmp);
			ft_strdel(&str);
			ft_strdel(&tmp);
			str = cache;
		}
		if (p->next)
		{
			cache = ft_strjoin(str, " | ");
			ft_strdel(&str);
			str = cache;
		}
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
	if (sig == 0 && job_is_completed(j))
		str = ft_strdup("Terminated: 15");
	else if (sig == 0 && (!job_is_completed(j)) && (!job_is_stopped(j)))
		str = ft_strdup("Running");
	else if (sig == 1)
		str = ft_strdup("Terminated(SIGHUP)");
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

char		*ft_name_sig(int sig)
{
	char	*str;

	str = NULL;
	if (sig == 0)
		str = ft_strdup("Running");
	else if (sig >= 1 && sig <= 15)
		str = ft_strdup("Terminated");
	else if (sig == 17 || sig == 18 || sig == 21 || sig == 22)
		str = ft_strdup("Stopped");
	else if (sig == 19)
		str = ft_strdup("Running");
	return (str);
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

char		value_char_job(int current, int max)
{
	if (current == max)
		return ('+');
	if (current == max - 1)
		return ('-');
	return (' ');
}

static void	bt_jobs_l(t_job *j, int max_current, t_redirection *r)
{
	char		*cmd;
	char		*name_sig;
	t_process	*p;
	char		c;
	int			num_sig;

	p = j->first_process;
	c = value_char_job(j->current, max_current);
	ft_dprintf(r->out, "[%d]%c", j->process_id, c);
	while (p)
	{
		num_sig = (p->status < 32) ? p->status : WSTOPSIG(p->status);
		name_sig = ft_name_sig(num_sig);
		cmd = assemply_cmd_process(p);
		if (num_sig > 0)
			ft_dprintf(r->out, "\t%d %s: %d\t%s", p->pid, name_sig, num_sig, cmd);
		else
			ft_dprintf(r->out, "\t%d %s\t%s", p->pid, name_sig, cmd);
		if (p->next)
			ft_dprintf(r->out, " |\n");
		else
			ft_dprintf(r->out, "\n");
		ft_strdel(&name_sig);
		ft_strdel(&cmd);
		p = p->next;
	}
}

void		bt_jobs_s(t_job *j, int max_current, t_redirection *r)
{
	t_process	*p;
	char		*str;
	char		*cmd;
	char		c;

	p = j->first_process;
	while (p)
	{
		if (!p->next)
			break ;
		p = p->next;
	}
	str = ft_inter_signal(WSTOPSIG(p->status), j);
	if (!str)
		return ;
	cmd = cmd_job_s(j);
	c = value_char_job(j->current, max_current);
	ft_dprintf(r->out, "[%d]%c\t%s\t%s\n", j->process_id, c, str, cmd);
	ft_strdel(&str);
	ft_strdel(&cmd);
}

static t_job	*last_jobs(void)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current;
	j = get_first_job(NULL);
	while (j)
	{
		if (j->current == max_current)
			return (j);
		j = j->next;
	}
	return (NULL);
}

static t_job	*previous_jobs(void)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current - 1;
	j = get_first_job(NULL);
	while (j)
	{
		if (j->current == max_current)
			return (j);
		j = j->next;
	}
	return (NULL);
}

t_job	*ft_search_pid_job(pid_t pid)
{
	t_job	*job;

	job = get_first_job(NULL);
	while (job)
	{
		if (job->pgid == pid)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*ft_search_id_job(char *av, int index)
{
	t_job	*job;
	int		id;

	id = ft_atoi(av + index);
	job = get_first_job(NULL);
	while (job)
	{
		if (job->process_id == id)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*ft_search_str_job(char *av, int index)
{
	t_job	*job;
	t_job	*final;
	char	*str;

	job = get_first_job(NULL);
	final = NULL;
	while (job)
	{
		str = ft_strnstr(job->first_process->cmd[0], av + index,
				ft_strlen(av + index));
		if (str)
		{
			if (final)
				return (NULL);
			else
				final = job;
		}
		job = job->next;
	}
	return (final);
}

t_job	*ft_search_exist_job(char *av, int index)
{
	t_job	*job;
	t_job	*final;
	char	*str;
	int		id;

	id = ft_atoi(av + index);
	job = get_first_job(NULL);
	final = NULL;
	while (job)
	{
		str = ft_strstr(job->first_process->cmd[0], av + index + 1);
		if (str)
		{
			if (final)
				return (NULL);
			else
				final = job;
		}
		job = job->next;
	}
	return (final);
}

static int		ft_good_syntax(char *av)
{
	int	i;
	int	percent;

	i = 0;
	if (av[i] == '%')
		++i;
	percent = (i > 0) ? -1 : 1;
	if (!av[i])
		return (4);
	if (av[i] == '%' || av[i] == '+')
		return ((!av[i + 1]) ? 1 * percent : 4 * percent);
	if (av[i] == '-')
		return ((!av[i + 1]) ? 2 * percent : 4 * percent);
	if (av[i] == '?')
		return (5 * percent);
	return (ft_isnumbers(av + i) ? 3 * percent : 4 * percent);
}

static t_job	*search_job(char *av)
{
	int	job_id;

	job_id = ft_good_syntax(av);
	if (job_id == 1 || job_id == -1)
		return (last_jobs());
	else if (job_id == 2 || job_id == -2)
		return (previous_jobs());
	else if (job_id == 3 || job_id == -3)
		return (ft_search_id_job(av, (job_id > -1) ? 0 : 1));
	else if (job_id == 4 || job_id == -4)
		return (ft_search_str_job(av, (job_id > -1) ? 0 : 1));
	else if (job_id == 5 || job_id == -5)
		return (ft_search_exist_job(av, (job_id > -1) ? 0 : 1));
	else
		return (NULL);
}

static void	display_jobs_options(void (*p)(t_job*, int, t_redirection*),
	t_redirection *r, char **av)
{
	t_job	*j;
	int		i;
	int		verif;
	int		max_current;

	max_current = get_shell()->max_job_current;
	i = (ft_strequ(*av, "--")) ? 1 : 0;
	while (av[i])
	{
		verif = 0;
		j = search_job(av[i]);
		if (j)
		{
			verif = 1;
			if (job_is_completed(j) || job_is_stopped(j))
			{
				(*p)(j, max_current, r);
			}
			else
				display_no_such_job(r, av[i]);
		}
		else
			display_no_such_job(r, av[i]);
		++i;
	}
}

static void	display_jobs(void (*p)(t_job*, int, t_redirection*),
	t_redirection *r, char **av)
{
	t_job	*j;
	int		max_current;

	max_current = get_shell()->max_job_current;
	if (!*av)
	{
		j = get_first_job(NULL);
		while (j)
		{
			if (job_is_completed(j) || job_is_stopped(j) || j->pgid > 0)
				(*p)(j, max_current, r);
			j = j->next;
		}
	}
	else
		display_jobs_options(p, r, av);
}

int			bt_jobs(t_job *j, char **av, t_redirection *r)
{
	void	(*p)(t_job*, int, t_redirection*);

	if (j->fg == 0)
	{
		display_no_job_control(r, "jobs");
		return (1);
	}
	update_status();
	p = &bt_jobs_s;
	while (*(++av))
	{
		if (ft_strequ(*av, "-p"))
			p = &bt_jobs_p;
		else if (ft_strequ(*av, "-l"))
			p = &bt_jobs_l;
		else if (ft_strequ(*av, "--") || ft_strchr_exist(*av, '%'))
			break ;
		else
		{
			display_invalid_option_job(r, *av);
			return (-2);
		}
	}
	display_jobs(p, r, av);
	return (0);
}

t_job		*job_for_bg_fg(char **av, t_redirection *r)
{
	t_job	*j;
	t_job	*job_launch;
	int		i;

	update_status();
	j = get_first_job(NULL);
	job_launch = NULL;
	if (!av[1])
	{
		while (j)
		{
			if (j->first_process->stopped == 1 || j->pgid > 0)
				job_launch = j;
			j = j->next;
		}
	}
	else
	{
		i = 1;
		if (ft_strequ(*av, "--"))
			++i;
		while (av[i])
		{
			j = search_job(av[i]);
			if (j)
			{
				if (job_is_completed(j) || job_is_stopped(j) || j->pgid > 0)
					job_launch = j;
				else
					display_no_such_job(r, av[i]);
			}
			else
				display_no_such_job(r, av[i]);
			++i;
		}
	}
	return (job_launch);
}

void		change_fg_var_job(t_job *j, int fg)
{
	t_process	*p;

	j->fg = fg;
	p = j->first_process;
	while (p)
	{
		p->fg = fg;
		p = p->next;
	}
}

int			bt_bg(t_job *j, char **av, t_redirection *r)
{
	char		*cmd;
	t_job		*job_launch;

	if (j->fg == 0)
	{
		display_no_job_control(r, "bg");
		return (1);
	}
	job_launch = job_for_bg_fg(av, r);
	if (!job_launch)
	{
		display_no_current_job(r, "bg");
		return (-2);
	}
	change_fg_var_job(job_launch, 0);
	cmd = cmd_job_s(job_launch);
	if (cmd)
		ft_dprintf(STDOUT_FILENO, "%s\n", cmd);
	ft_strdel(&cmd);
	continue_job(job_launch, 0);
	return (0);
}

int			bt_fg(t_job *j, char **av, t_redirection *r)
{
	char	*cmd;
	t_job	*job_launch;

	if (j->fg == 0)
	{
		display_no_job_control(r, "fg");
		return (1);
	}
	job_launch = job_for_bg_fg(av, r);
	if (!job_launch)
	{
		display_no_current_job(r, "fg");
		return (-2);
	}
	change_fg_var_job(job_launch, 1);
	cmd = cmd_job_s(job_launch);
	if (cmd)
		ft_dprintf(STDOUT_FILENO, "%s\n", cmd);
	ft_strdel(&cmd);
	continue_job(job_launch, 1);
	return (0);
}
