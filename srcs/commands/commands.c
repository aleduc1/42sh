/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:22:31 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"
#include "builtins.h"

/*
** command error
*/

void		display_error_command(t_redirection *r, char **cmd)
{
	ft_dprintf(r->error, "42sh: command not found: %s\n", cmd[0]);
}

/*
** simple command
*/

void		display_lst_job(t_job *j)
{
	t_job		*sv;
	t_process	*p;

	sv = j;
	if (!sv)
		return ;
	while (sv)
	{
		ft_printf("===========================\n");
		ft_printf("pgpid = %d\nnotified = %d\n", sv->pgid, sv->notified);
		p = sv->first_process;
		while (p)
		{
			ft_printf("___________________________\n");
			ft_arraydisplay(p->cmd);
			ft_printf("pointeur:\n cmd = %p\n cmd_path = %p\n", &p->cmd,
					&p->cmd_path);
			ft_printf("redirection = %p\n", &p->r);
			ft_printf("pid = %d\ncompleted = %d\n", p->pid, p->completed);
			ft_printf("stopped = %d\nstatus = %d\n", p->stopped,
					p->status);
			p = p->next;
		}
		sv = sv->next;
	}
}

int			condition_clean_list(t_job *j, pid_t pid)
{
	if ((((j->first_process->completed || j->first_process->pid == 0)
		&& j->first_process->stopped == 0))
		|| j->first_process->pid == pid)
		return (1);
	return (0);
}

void		clean_fuck_list(pid_t pid)
{
	t_job	**j;
	t_job	*last;
	t_job	*next;
	t_job	*h;

	j = static_job();
	h = *j;
	last = NULL;
	while (*j)
	{
		next = (*j)->next;
		if (condition_clean_list(*j, pid))
		{
			if (last)
				last->next = next;
			else
				h = NULL;
			free_job(&(*j));
		}
		else
			last = (*j);
		(*j) = next;
	}
	(*j) = h;
}

static int	file_to_close_bis(t_token *t, t_job *j)
{
	t_lex	*lex;
	int		i;

	lex = t->command;
	if (!(j->close_fd = (int*)malloc(sizeof(int) * (j->len_close + 1))))
		return (-1);
	i = -1;
	while (lex)
	{
		if (lex->token->type == REDIR && lex->redir &&
				lex->redir->dest_fd &&
				ft_atoi(lex->redir->dest_fd) != -1)
			if (lex->redir->filename || lex->redir->close == 1)
				j->close_fd[++i] = ft_atoi(lex->redir->dest_fd);
		lex = lex->next;
	}
	return (0);
}

int			file_to_close(t_token *t, t_job *j)
{
	t_lex	*lex;
	int		verif;

	if (!t)
		return (-1);
	lex = t->command;
	while (lex)
	{
		if (lex->token->type == REDIR && lex->redir &&
				lex->redir->dest_fd &&
				ft_atoi(lex->redir->dest_fd) != -1)
			if (lex->redir->filename || lex->redir->close == 1)
				j->len_close++;
		lex = lex->next;
	}
	verif = file_to_close_bis(t, j);
	return (verif);
}

t_job		*create_new_job(char **argv, t_token *t, t_redirection *r, int fg)
{
	t_job			*j;
	t_process		*p;
	int				process_id;

	j = get_first_job(NULL);
	process_id = 0;
	while (j && j->pgid != 0)
	{
		process_id = j->first_process ? j->first_process->process_id : 0;
		if (!j->next)
			j->next = init_job();
		j = j->next;
	}
	file_to_close(t, j);
	j->pgid = 0;
	j->notif_stop = 0;
	j->r = base_redirection();
	p = j->first_process;
	p->cmd = ft_arraydup(argv);
	parser_var(&p->cmd);
	p->process_id = process_id + 1;
	p->fg = fg;
	p->r = (t) ? fill_redirection(t) : r;
	return (j);
}

int			ft_simple_command(char **argv, t_token *t, t_pos *pos)
{
	int				verif;
	t_job			*j;
	t_process		*p;

	verif = 0;
	j = create_new_job(argv, t, NULL, 1);
	p = j->first_process;
	if (check_last_command() == -5)
	{
		gest_return(1);
		clean_fuck_list(0);
		return (1);
	}
	if ((verif = is_builtin(j, p, pos)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		if (p->cmd_path)
			verif = launch_job(j, 1);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	clean_fuck_list(0);
	return (verif);
}

void		free_process_redirection(t_job **j)
{
	t_process	*p;
	t_process	*next;

	if ((!j) || (!(*j)))
		return ;
	p = (*j)->first_process;
	while (p)
	{
		next = p->next;
		ft_strdel(&(p->cmd_path));
		if (p->cmd)
			ft_arraydel(&(p->cmd));
		free(p);
		p = NULL;
		p = next;
	}
}

void		free_job_redirection(t_job **j)
{
	if (j && (*j))
	{
		if ((*j)->first_process)
			free_process_redirection(j);
		free(*j);
		(*j) = NULL;
	}
}

int			ft_simple_command_redirection(char **av, t_redirection *r, t_pos *pos)
{
	int				verif;
	t_job			*j;
	t_process		*p;

	verif = 0;
	j = init_job();
	j->first_process->cmd = ft_arraydup(av);
	j->next = NULL;
	p = j->first_process;
	p->r = r;
	// j = create_new_job(av, NULL, r, 1);
	// p = j->first_process;
	if (check_last_command() == -5)
	{
		gest_return(1);
		clean_fuck_list(0);
		return (1);
	}
	if ((verif = is_builtin(j, p, pos)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		if (p->cmd_path)
			verif = launch_job(j, 1);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	free_job_redirection(&j);
	// clean_fuck_list(0);
	return (verif);


	// int				verif;
	// t_job			*j;
	// t_process		*p;

	// verif = 0;
	// j = init_job();
	// j->first_process->cmd = ft_arraydup(av);
	// j->next = NULL;
	// p = j->first_process;
	// p->r = r;
	// if ((verif = is_builtin(j, p, NULL)) == -1)
	// {
	// 	p->cmd_path = is_in_path(p->cmd[0]);
	// 	if (p->cmd_path)
	// 		verif = launch_job(j, 1);
	// 	else
	// 		verif = gest_error_path(p->cmd[0], p->r);
	// }
	// ft_arraydel(&p->cmd);
	// free(p);
	// p = NULL;
	// free(j);
	// j = NULL;
	// gest_return(verif);
	// return (verif);
}

/*
** ||
*/

int			ft_pipe_double(char **argv, t_token *token, t_pos *pos)
{
	int		check;
	char	*str;

	str = value_line_path("?", 0);
	check = ft_atoi(str);
	if (check == -1)
		check = ft_simple_command(argv, token, pos);
	ft_strdel(&str);
	return (check);
}

/*
** &&
*/

int			ft_ampersand_double(char **argv, t_token *token, t_pos *pos)
{
	int		check;
	char	*str;

	str = value_line_path("?", 0);
	check = ft_atoi(str);
	if (check != -1)
		check = ft_simple_command(argv, token, pos);
	ft_strdel(&str);
	return (check);
}
