/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/01 16:13:04 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "job.h"

int			gest_error_path(char *cmd, t_redirection *r)
{
	if (check_last_command() == -6)
	{
		ft_dprintf(r->error, "21sh: %s: Permission denied\n", cmd);
		gest_return(126);
		return (126);
	}
	ft_dprintf(r->error, "21sh: command not found: %s\n", cmd);
	gest_return(127);
	return (127);
}

/*
** search if it's a builtin
**	return 0 if it's a builtin and if it's command work
**	return -2 if it's a builtin and if it's command not work
**	return -1 if it's not a builtin
*/

int			is_builtin_env(t_process *p, char **av)
{
	int	verif;

	if (ft_strequ(av[0], "env"))
		verif = builtin_env(p->r, av);
	else if (ft_strequ(av[0], "set"))
		verif = builtin_set(p->r);
	else if (ft_strequ(av[0], "setenv"))
		verif = edit_setenv(av[1], av[2]);
	else if (ft_strequ(av[0], "unsetenv"))
		verif = ft_unsetenv(av[1]);
	else if (ft_strequ(av[0], "export"))
		verif = bt_export(av + 1);
	else if (ft_strequ(av[0], "unset"))
		verif = ft_unset(av[1]);
	else if (ft_strchr_exist(av[0], '='))
		verif = edit_set(av, p->r);
	else
		verif = -1;
	return (verif);
}

int			is_builtin(t_job *j, t_process *p, t_pos *pos)
{
	int		verif;
	char	**av;

	av = p->cmd;
	verif = is_builtin_env(p, av);
	if (verif != -1)
		return (verif);
	if (ft_strequ(av[0], "echo"))
		verif = bt_echo(av, p->r);
	else if (ft_strequ(av[0], "cd"))
		verif = (builtin_cd(av) < 0) ? -2 : 0;
	else if (ft_strequ(av[0], "exit"))
		verif = bt_exit(j);
	else if (ft_strequ(av[0], "jobs"))
		verif = bt_jobs(av, p->r);
	else if (ft_strequ(av[0], "fg"))
		verif = bt_fg();
	else if (ft_strequ(av[0], "bg"))
		verif = bt_bg();
	else if (ft_strequ(av[0], "fc"))
		verif = builtin_fc(av, pos);
	else if (ft_strequ(av[0], "test"))
		verif = builtin_fc(av, p->r);
	else
		verif = -1;
	return (verif);
}

/*
** edit the local variable ('?') for know if the last command
** has worked
*/

int			gest_return(int verif)
{
	char	*value;

	if (verif > 255)
		verif %= 255;
	value = ft_itoa(verif);
	verif = add_set_value("?", value);
	ft_strdel(&value);
	return (verif);
}

/*
** is_end = 1 -> free env
** f_line is not null -> modify head env by f_line
*/

t_env		*get_env(int is_end, t_env *head)
{
	static t_env	*my_env;

	if (is_end == 3 && (!my_env))
		return (NULL);
	if ((!my_env) && (!head))
	{
		my_env = init_env();
		init_variable();
	}
	if (head)
		my_env = head;
	if (is_end == 1)
		free_env(&my_env);
	return (my_env);
}
