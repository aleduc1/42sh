/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/01 12:48:33 by mbellaic         ###   ########.fr       */
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
** remplacer 1 par 0 pour lancer en background
*/

int			ft_simple_command(char **argv, t_token *t, t_pos *pos)
{
	int				fg;
	int				verif;
	t_job			*j;
	t_process		*p;

	fg = 1;
	verif = 0;
	j = create_new_job(argv, t, NULL, fg);
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
			verif = launch_job(j, fg);
		else
			verif = gest_error_path(p->cmd[0], p->r);
	}
	clean_fuck_list(0);
	return (verif);
}

/*
** simple command for set or env
*/

int			ft_simple_command_redirection(char **av, t_redirection *r,
				t_pos *pos)
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
	if (check_last_command() == -5)
	{
		gest_return(1);
		clean_fuck_list(0);
		return (1);
	}
	if ((verif = is_builtin(j, p, pos)) == -1)
	{
		p->cmd_path = is_in_path(p->cmd[0]);
		verif = (p->cmd_path) ? launch_job(j, 1)
			: gest_error_path(p->cmd[0], p->r);
	}
	free_job_redirection(&j);
	return (verif);
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
