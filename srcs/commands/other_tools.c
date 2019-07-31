/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:57:48 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/05 02:17:05 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "job.h"

int			gest_error_path(char *cmd, t_redirection *r)
{
	if (check_last_command() == -6)
	{
		ft_dprintf(r->error, "42sh: %s: Permission denied\n", cmd);
		gest_return(126);
		return (126);
	}
	ft_dprintf(r->error, "42sh: command not found: %s\n", cmd);
	gest_return(127);
	return (127);
}

/*
** edit the local variable ('?') for know if the last command
** has worked
*/

int			gest_return(int verif)
{
	char	*value;

	if (verif == 13)
		return (127);
	if (verif > 1 && verif < 23)
		verif = 128 + verif;
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
