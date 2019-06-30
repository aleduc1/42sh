/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 10:50:50 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:50:48 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "job.h"


// int			exec_pipe(t_process *p, int in, int out)
// {
// 	char	**environ;
// 	char	*str;

// 	str = is_in_path(p->av[0]);
// 	if (!str)
// 	{
// 		gest_return(gest_error_path(p->av[0], p->r));
// 		return (-1);
// 	}
// 	if (p->r->in == STDIN_FILENO)
// 		p->r->in = in;
// 	if (p->r->out == STDOUT_FILENO)
// 		p->r->out = out;
// 	environ = create_list_env(get_env(0, NULL), 0);
// 	redirection_fd(p->r);
// 	execve(str, p->av, environ);
// 	ft_dprintf(p->r->error, "21sh: command not found: %s\n", p->av[0]);
// 	ft_arraydel(&environ);
// 	ft_strdel(&str);
// 	execve("/bin/test", NULL, NULL);
// 	exit(0);
// 	return (0);
// }

int			ft_pipe(char **argv, t_token *token, int end_pipe)
{
	t_job		*j;
	t_process	*p;

	if (end_pipe == 0)
		create_new_job(argv, token);
	else
		add_process(argv, token);
	if (end_pipe == 2)
	{
		j = get_end_job();
		launch_job_pipe(j, 1);
		if (j->first_process->pid == 0)
			clean_fuck_list(0);
		p = j->first_process;
		while (p)
		{
			if (p->completed)
			{
				clean_fuck_list(j->first_process->pid);
				break ;
			}
			p = p->next;
		}
		update_status();
	}
	return (0);
}
