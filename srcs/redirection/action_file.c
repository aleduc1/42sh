/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 16:44:29 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/07 17:41:28 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/*
** F_OK: file exist
*/

int	file_exist(char *name, int type)
{
	int		fd;

	if (type == LESSAMP)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: Ambiguous redirect\n", name);
		return (-1);
	}
	if (access(name, F_OK) != -1)
		return (0);
	if (type == LESS)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: No such file or directory\n",
					name);
		return (-1);
	}
	fd = open(name, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	ft_dprintf(STDERR_FILENO, "42sh: Error create file: %s\n", name);
	return (-1);
}

/*
** O_TRUNC -> remove data in file
** LINE 57: return (fcntl(ft_atoi(redir->dest_fd), F_GETFD));
*/

int	open_file_great(t_redir *redir)
{
	if (redir->filename)
		redir->dest_fd = ft_itoa(file_exist(redir->filename, redir->type));
	else
		return (0);
	if (ft_atoi(redir->dest_fd) == -1)
		return (-1);
	ft_strdel(&redir->dest_fd);
	if (redir->type == DGREAT)
		redir->dest_fd = ft_itoa(get_end_line(redir->filename));
	else if (redir->type == GREAT || redir->type == DLESS
		|| redir->type == AMPGREAT || redir->type == GREATAMP)
		redir->dest_fd = ft_itoa(open(redir->filename, O_RDWR | O_TRUNC));
	else
		redir->dest_fd = ft_itoa(open(redir->filename, O_RDWR));
	return (1);
}

int	open_file_dless(t_redir *redir, t_pos *pos)
{
	int		fd;
	char	*name;
	char	*str;
	char	*tmp;

	if (!(redir->heredoc))
		return (-1);
	name = ft_strdup("/tmp/.42sh0");
	redir->filename = name;
	tmp = ft_strdup(redir->heredoc);
	str = heredoc(tmp, pos);
	if (!str)
		return (-1);
	ft_remove_last_chr(&str);
	fd = file_exist(name, redir->type);
	(fd > -1) ? fd = open_file_great(redir) : 0;
	if (fd <= -1)
		return (-1);
	ft_dprintf(ft_atoi(redir->dest_fd), "%s", str);
	if (verif_close(ft_atoi(redir->dest_fd)))
		close(ft_atoi(redir->dest_fd));
	ft_strdel(&(redir->dest_fd));
	redir->dest_fd = ft_itoa(open(redir->filename, O_RDWR));
	ft_strdel(&str);
	return (1);
}
