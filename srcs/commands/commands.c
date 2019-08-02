/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/02 16:34:18 by apruvost         ###   ########.fr       */
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
	ft_dprintf(r->error, "21sh: command not found: %s\n", cmd[0]);
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
		ft_printf("===========================\n");
	}
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
		if (((((*j)->first_process->completed || (*j)->first_process->pid == 0)
		&& (*j)->first_process->stopped == 0))
		|| (*j)->first_process->pid == pid)
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

t_redirection	*base_redirection(void)
{
	t_redirection	*r;

	if (!(r = (t_redirection*)malloc(sizeof(t_redirection) * 1)))
		return (NULL);
	r->in = STDIN_FILENO;
	r->out = STDOUT_FILENO;
	r->error = STDERR_FILENO;
	r->redirect = NULL;
	return (r);
}

t_job		*edit_lst_job(char **argv, t_token *t, t_redirection *r)
{
	t_job			*j;
	t_process		*p;
	int				process_id;

	j = get_first_job(NULL);
	process_id = 0;
	while (j->pgid != 0)
	{
		process_id = j->first_process->process_id;
		if (!j->next)
			j->next = init_job();
		j = j->next;
	}
	file_to_close(t, j);
	j->pgid = 0;
	j->r = base_redirection();
	p = j->first_process;
	p->cmd = ft_arraydup(argv);
	p->process_id = process_id + 1;
	parser_var(&p->cmd);
	if (t)
		p->r = fill_redirection(t);
	else
		p->r = r;
	return (j);
}

int			ft_simple_command(char **argv, t_token *t, t_pos *pos)
{
	int				verif;
	t_job			*j;
	t_process		*p;

	verif = 0;
	j = edit_lst_job(argv, t, NULL);
	p = j->first_process;
	if ((verif = is_builtin(j, p, pos)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0], 1);
		if (p->cmd_path)
			verif = launch_job(j, 1);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	if (p->completed == 1 || p->pid == 0 || p->stopped == 0)
		clean_fuck_list(0);
	return (verif);
}

int			ft_simple_command_redirection(char **av, t_redirection *r)
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
	if ((verif = is_builtin(j, p, NULL)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0], 1);
		if (p->cmd_path)
			verif = launch_job(j, 1);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	ft_arraydel(&p->cmd);
	free(p);
	p = NULL;
	free(j);
	j = NULL;
	gest_return(verif);
	return (verif);
}

/*
** ||
*/

int			ft_pipe_double(char **argv, t_token *token)
{
	int		check;
	char	*str;

	str = value_line_path("?", 0);
	check = ft_atoi(str);
	if (check == -1)
		check = ft_simple_command(argv, token, NULL);
	ft_strdel(&str);
	return (check);
}

/*
** &
** num_process: processus en cours (mettre a 1 la premiere fois)
*/

int			ft_ampersand(char **argv, t_token *token)
{
	int				verif;
	t_job			*j;
	t_process		*p;

	verif = 0;
	j = edit_lst_job(argv, token, NULL);
	p = j->first_process;
	if ((verif = is_builtin(j, p, NULL)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0], 1);
		if (p->cmd_path)
			verif = launch_job(j, 1);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	if (p->completed == 1 || p->pid == 0)
		clean_fuck_list(0);
	gest_return(verif);
	return (verif);
}

/*
** &&
*/

int			ft_ampersand_double(char **argv, t_token *token)
{
	int		check;
	char	*str;

	str = value_line_path("?", 0);
	check = ft_atoi(str);
	if (check != -1)
		check = ft_simple_command(argv, token, NULL);
	ft_strdel(&str);
	return (check);
}
