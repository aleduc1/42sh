/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 09:17:14 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 13:20:40 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef JOB_H
#	define JOB_H

#include "sh21.h"
#include "env.h"

t_job		*edit_lst_job(char **argv, t_token *t, t_redirection *r);

/*
** shell_struct.c
*/

t_shell			**static_shell(void);
t_shell			*get_shell(void);
void			delete_shell(void);

/*
** signaux.c
*/

void			ign_signaux(void);
void			dfl_signaux(void);

/*
** tools_job.c
*/

t_job			*get_first_job(t_job *new_job);
t_job			*find_job(pid_t pid);
int				job_is_completed(t_job *job);
int				job_is_stop(t_job *job);
t_job			*init_job(void);
t_process		*init_process(void);
void			free_job(t_job **j);
t_job			**static_job(void);
void			free_all_job(void);

/*
** process.c
*/

int				launch_job(t_job *j, int fg);
int				launch_process(t_process *p, pid_t pgid, t_redirection *r, int fg);
void			act_job(t_job *j, int fg);
int 			launch_job_pipe(t_job *j, int fg);

/*
** add_job.c
*/

void			add_in_fg(t_job *j, int value);
void			add_in_bg(t_job *j, int value);

/*
** gest_process.c
*/

void			continue_job(t_job *j, int fg);
void			job_running(t_job *j);
void			job_notif(void);
void			job_info(t_job *j, char *status);
void			wait_for_jobs(t_job *j);
void			update_status(void);
int				mark_process_status(pid_t pid, int status);

void		display_lst_job(t_job *j);

void		clean_fuck_list(void);

#	endif
