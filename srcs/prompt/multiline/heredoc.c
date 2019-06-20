/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:01:09 by aleduc            #+#    #+#             */
/*   Updated: 2019/05/15 16:31:34 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void		init_heredoc(t_pos *pos)
{
	pos->multiline = 1;
	pos->nblines = 0;
	pos->currentline = 0;
	pos->selection = 0;
	pos->selectcount = 0;
	pos->inputlen = 0;
	pos->savecolumn = 0;
	pos->saverow = 0;
	pos->startrow = 0;
	pos->startcolumn = 0;
	pos->tailcolumn = 0;
	pos->tailrow = 0;
	pos->stop = 0;
}

char		*heredoc_string(t_node *lstcursor)
{
	char	*temp;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (lstcursor->next)
	{
		len++;
		lstcursor = lstcursor->next;
	}
	if (!(temp = malloc(sizeof(char) * len + 1)))
		return (NULL);
	while (lstcursor->prev)
	{
		temp[i] = lstcursor->key;
		lstcursor = lstcursor->prev;
		i++;
	}
	temp[i] = '\0';
	return (temp);
}

int			check_heredoc(t_node *input, char *heredoc)
{
	char	*temp;
	t_node	*lstcursor;

	lstcursor = input;
	temp = heredoc_string(lstcursor);
	if (ft_strcmp(heredoc, temp) == 0)
	{
		free(temp);
		return (1);
	}
	free(temp);
	return (-1);
}

int			input_heredoc(t_multi *lstcursor, t_multi **multi, t_pos *pos, \
																char *heredoc)
{
	lstcursor = *multi;
	multi_push(multi);
	lstcursor = lstcursor->prev;
	lstcursor->input = NULL;
	dpush(&lstcursor->input, ' ');
	ft_putstr("heredoc>");
	read_input(&lstcursor->input, pos);
	return (check_heredoc(lstcursor->input, heredoc));
}

char		*heredoc(char *heredoc, t_pos *pos)
{
	t_multi	*multi;
	t_multi	*lstcursor;
	char	*input;

	multi = NULL;
	input = NULL;
	lstcursor = NULL;
	raw_term_mode();
	multi_push(&multi);
	multi->input = NULL;
	dpush(&multi->input, ' ');
	init_heredoc(pos);
	while (((input_heredoc(lstcursor, &multi, pos, heredoc)) < 0) \
			&& pos->stop != 1)
		init_heredoc(pos);
	input = lst_to_str(&multi, input);
	ddellist(multi);
	if (pos->stop == 1)
	{
		pos->stop = 0;
		ft_strdel(&input);
	}
	default_term_mode();
	return (input);
}
