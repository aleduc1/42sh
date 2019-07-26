/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 16:44:29 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/07 17:41:28 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/*
** F_OK: file exist
*/

int			file_exist(char *name, int type)
{
	int		fd;

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
	ft_printf("42sh: error create file: %s\n", name);
	return (-1);
}

/*
** O_TRUNC -> remove data in file
*/

static int	open_file_great(t_redir *redir)
{
	if (redir->filename)
		redir->dest_fd = ft_itoa(file_exist(redir->filename, redir->type));
	else
		return (fcntl(ft_atoi(redir->dest_fd), F_GETFD));
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

static int	open_file_dless(t_redir *redir, t_pos *pos)
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
	close(ft_atoi(redir->dest_fd));
	ft_strdel(&(redir->dest_fd));
	redir->dest_fd = ft_itoa(open(redir->filename, O_RDWR));
	ft_strdel(&str);
	return (1);
}

int			close_file_command(t_lex *lex, t_redirection **r)
{
	t_lex	*head;

	head = lex;
	while (lex)
	{
		if (lex->token->type == REDIR && lex->redir
			&& lex->redir->dest_fd
			&& ft_atoi(lex->redir->dest_fd) != -1)
			if (lex->redir->filename || lex->redir->close == 1)
				close(ft_atoi(lex->redir->dest_fd));
		lex = lex->next;
	}
	lex = head;
	delete_redirection(&(*r));
	return (0);
}

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
	if (redir->filename)
		parser_var_simple(&redir->filename);
	if (redir->type == GREAT || redir->type == DGREAT || redir->type == LESS
		|| redir->type == AMPGREAT || redir->type == AMPLESS
		|| redir->type == LESSAMP || redir->type == GREATAMP)
	{
		verif = open_file_great(redir);
		// if ((verif = open_file_great(redir)) == -1)
		// 	ft_dprintf(STDERR_FILENO, "42sh: bad file descriptor\n");
	}
	else if (redir->type == DLESS)
	{
		if ((verif = open_file_dless(redir, pos)) == -1)
			ft_dprintf(STDERR_FILENO, "42sh: bad file descriptor\n");
	}
	if (verif > -1)
		verif = 0;
	return (verif);
}
