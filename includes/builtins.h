/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 16:01:05 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/06 18:36:47 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef BUILTINS_H
#	define BUILTINS_H

#include "env.h"
#include "job.h"

typedef struct		s_cd
{
	int		arg__;
	char	*directory;
	char	*curpath;
}					t_cd;

int					bt_exit(t_job *j);
int					bt_echo(char **av, t_redirection *r);

/*
** bt_jobs.c
*/

int					bt_jobs(char **av, t_redirection *r);
int					bt_bg(void);
int					bt_fg(void);
int					bt_export(char **value);

int					builtin_fc(char **av, t_pos *pos);

#	endif
