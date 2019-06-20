/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 10:40:08 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 12:02:22 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "sh21.h"

/*
** tcgetattr(shell->term, &(shell->term_shell));
*/

static t_shell	*init_shell_list(void)
{
	t_shell			*shell;

	if (!(shell = (t_shell*)malloc(sizeof(t_shell) * 1)))
		return (NULL);
	return (shell);
}

t_shell			**static_shell(void)
{
	static t_shell	*shell;

	if (!shell)
		shell = init_shell_list();
	return (&shell);
}

t_shell			*get_shell(void)
{
	t_shell **shell;

	shell = static_shell();
	return (*shell);
}

void			delete_shell(void)
{
	t_shell	**shell;

	shell = static_shell();
	free(*shell);
	*shell = NULL;
}
