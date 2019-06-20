/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 03:02:22 by mbellaic          #+#    #+#             */
/*   Updated: 2019/06/06 18:36:57 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh21.h"
#include "env.h"

int			get_argc(t_lex *cursor)
{
	int		len;

	len = 0;
	while (cursor)
	{
		if (cursor->token->type == WORD || cursor->token->type == NUMBER)
			len++;
		cursor = cursor->next;
	}
	return (len);
}

char		**get_argv(t_token *cmd_list)
{
	char	**argv;
	t_lex	*cursor;
	int		len;
	int		i;

	i = 0;
	cursor = cmd_list->command;
	len = get_argc(cursor);
	if (!(argv = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	while (cursor)
	{
		if (cursor->token->type == WORD || cursor->token->type == NUMBER)
			argv[i++] = ft_strdup(cursor->token->data);
		cursor = cursor->next;
	}
	argv[i] = NULL;
	return (argv);
}

void		files_handler(t_token *cmd_list, t_pos *pos)
{
	t_lex	*cursor;

	cursor = cmd_list->command;
	while (cursor)
	{
		if (cursor->token->type == REDIR)
			open_file_command(cursor->redir, pos);
		cursor = cursor->next;
	}
}

int			*run_pipe(t_token *cmd_list, t_pos *pos, int end_pipe)
{
	char	**argv;

	argv = get_argv(cmd_list);
	files_handler(cmd_list, pos);
	ft_pipe(argv, cmd_list, end_pipe);
	ft_arraydel(&argv);
	return (0);
}

int			*run_cmd(t_token *cmd_list, t_pos *pos)
{
	char	**argv;

	argv = get_argv(cmd_list);
	files_handler(cmd_list, pos);
	ft_simple_command(argv, cmd_list, pos);
	if (argv && *argv)
		ft_arraydel(&argv);
	return (0);
}
