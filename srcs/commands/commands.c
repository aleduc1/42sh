/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/19 01:18:10 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"
#include "builtins.h"

/*
** launch command in a fork if not builtin command
*/

static int		is_not_builtin(t_job *j, t_process *p, int fg)
{
	int	verif;
	int	last_return;

	verif = 0;
	p->cmd_path = is_in_path(p->cmd[0]);
	last_return = check_last_command();
	verif = launch_job(j, fg);
	if (last_return == -6)
		verif = gest_return(-6);
	if (!(p->cmd_path))
		verif = gest_error_path(p->cmd[0], p->r);
	return (verif);
}

/*
** Simple command
** Args: 	char **argv -> command
**			t_token *t ->
**			t_pos *pos -> know position terminal for fc builtin or heredocs
**			int bg -> if foreground bg = 0 or
**					if it's a background command -> ID background
*/

int				ft_simple_command(char **argv, t_token *t, t_pos *pos, int bg)
{
	int				fg;
	int				verif;
	t_job			*j;
	t_process		*p;

	if ((!argv) || (!*argv))
		return (-1);
	if (bg != 0)
		manage_id_job(bg);
	fg = (bg > 0) ? 0 : 1;
	verif = 0;
	j = create_new_job(argv, t, NULL, fg);
	p = j->first_process;
	if (check_last_command() == -5)
	{
		gest_return(1);
		clean_fuck_list(0);
		return (1);
	}
	if (!builtin_exist(p->cmd[0]))
		verif = is_not_builtin(j, p, fg);
	else
		verif = builtin(j, p, pos, fg);
	return (verif);
}

/*
** Copy struct redirection to ft_simple_command_redirection
*/

t_redirection	*cpy_redirection(t_redirection *r)
{
	t_redirection	*cpy;

	if (!(cpy = (t_redirection*)malloc(sizeof(t_redirection) * 1)))
		return (NULL);
	cpy->in = r->in;
	cpy->out = r->out;
	cpy->error = r->error;
	cpy->redirect = ft_init_redirect();
	while (r->redirect && r->redirect->base != -1)
	{
		ft_create_maillon_redirect(cpy->redirect, r->redirect->base,
			r->redirect->new_fd, r->redirect->type);
		r->redirect = r->redirect->next;
	}
	ft_create_maillon_redirect(cpy->redirect, -1,
			-1, -1);
	return (cpy);
}

/*
** Simple command for set or env
** give a job for is_not_builtin command
** Args:	char **argv -> command
**			t_redirection *r -> struct to redirection of the command
**			t_pos *pos -> know position terminal for fc builtin or heredocs
**			int fg -> if 0 it's a background command. If 1 it's a foreground
**				command.
*/

int				ft_simple_command_redirection(char **argv, t_redirection *r,
	t_pos *pos, int fg)
{
	int				verif;
	t_job			*j;
	t_process		*p;
	t_redirection	*cpy;

	verif = 0;
	cpy = cpy_redirection(r);
	j = create_new_job(argv, NULL, cpy, fg);
	p = j->first_process;
	if (check_last_command() == -5)
	{
		gest_return(1);
		clean_fuck_list(0);
		return (1);
	}
	if (!builtin_exist(p->cmd[0]))
		verif = is_not_builtin(j, p, fg);
	else
		verif = builtin(j, p, pos, fg);
	return (verif);
}
