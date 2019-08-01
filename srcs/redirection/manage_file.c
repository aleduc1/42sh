/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 16:44:29 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/07 17:41:28 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/*
** pos -> pour les heredocs
** LESS -> <
** DLESS -> <<
** GREAT -> >
** DGREAT -> >>
** AMPGREAT -> &>
** GREATAMP -> >&
** LESSAMP -> <&
** AMPLESS -> &<
*/

int			verif_file_descriptor(char **src, char *dst)
{
	int				i;
	struct rlimit	lim;
	uintmax_t		lim_cur;

	if (getrlimit(RLIMIT_NOFILE, &lim) == -1)
		return (-1);
	lim_cur = (uintmax_t)lim.rlim_cur;
	i = -1;
	while (src && src[++i])
		if ((uintmax_t)ft_atoi(src[i]) > lim_cur)
		{
			ft_dprintf(STDERR_FILENO, "42sh: Bad file descriptor\n");
			return (-1);
		}
	if (dst && (uintmax_t)ft_atoi(dst) > lim_cur)
	{
		ft_dprintf(STDERR_FILENO, "42sh: Bad file descriptor\n");
		return (-1);
	}
	return (0);
}

void		whois_type(int type)
{
	if (type == LESS)
		ft_printf("type is LESS\n");
	if (type == DLESS)
		ft_printf("type is DLESS\n");
	if (type == GREAT)
		ft_printf("type is GREAT\n");
	if (type == DGREAT)
		ft_printf("type is DGREAT\n");
	if (type == AMPGREAT)
		ft_printf("type is AMPGREAT\n");
	if (type == GREATAMP)
		ft_printf("type is GREATAMP\n");
	if (type == AMPLESS)
		ft_printf("type is AMPLESS\n");
	if (type == LESSAMP)
		ft_printf("type is LESSAMP\n");
}

static int	open_file_command_bis(t_redir **redir, t_pos *pos, int verif)
{
	if ((*redir)->filename)
		parser_var_simple(&((*redir)->filename));
	if ((*redir)->type == GREAT || (*redir)->type == DGREAT
		|| (*redir)->type == LESS || (*redir)->type == AMPGREAT
		|| (*redir)->type == AMPLESS || (*redir)->type == LESSAMP
		|| (*redir)->type == GREATAMP)
	{
		if (open_file_great(*redir) == -1)
		{
			gest_return(1);
			return (-1);
		}
	}
	else if ((*redir)->type == DLESS)
	{
		if ((verif = open_file_dless(*redir, pos)) == -1)
			ft_dprintf(STDERR_FILENO, "42sh: Bad file descriptor\n");
	}
	return (verif);
}

int			open_file_command(t_redir *redir, t_pos *pos)
{
	int	verif;

	verif = 0;
	if (!redir)
		return (-1);
	if (redir->type == LESSAMP && ft_strequ(redir->src_fd[0], "1"))
	{
		ft_strdel(&redir->src_fd[0]);
		redir->src_fd[0] = ft_strdup("0");
	}
	verif = open_file_command_bis(&redir, pos, verif);
	if (verif_file_descriptor(redir->src_fd, redir->dest_fd) == -1)
		verif = -1;
	if (verif > -1)
		verif = 0;
	else
		gest_return(1);
	return (verif);
}
