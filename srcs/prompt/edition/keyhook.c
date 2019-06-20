/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhook.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:18:30 by aleduc            #+#    #+#             */
/*   Updated: 2019/04/27 18:54:49 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

t_node		*up(t_node *lstcursor, t_node **input, char buffer[], t_pos *pos)
{
	t_node	*cursorhis;
	int		i;

	cursorhis = pos->history;
	if (ARROW_UP && pos->historycount < pos->historysum)
	{
		i = 1;
		clean_for_history(lstcursor, input, pos);
		pos->historycount++;
		while (i <= pos->historycount)
		{
			if (!(cursorhis->next))
				break ;
			cursorhis = cursorhis->next;
			i++;
		}
		lstcursor = history_to_lst(lstcursor, cursorhis->line, input, pos);
	}
	return (lstcursor);
}

t_node		*down(t_node *lstcursor, t_node **input, char buffer[], t_pos *pos)
{
	t_node	*cursorhis;
	int		i;

	cursorhis = pos->history;
	if (ARROW_DOWN && pos->historycount > 0)
	{
		if (pos->historycount == 1)
		{
			lstcursor = history_downcase(lstcursor, input, pos);
			return (lstcursor);
		}
		i = -1;
		lstcursor = clean_for_history(lstcursor, input, pos);
		pos->historycount--;
		while (++i < pos->historycount)
		{
			if (!(cursorhis->next))
				break ;
			cursorhis = cursorhis->next;
		}
		lstcursor = history_to_lst(lstcursor, cursorhis->line, input, pos);
	}
	return (lstcursor);
}

t_node		*left(t_node *lstcursor, char buffer[], t_pos *pos)
{
	if (ARROW_LEFT && lstcursor->next)
	{
		if (pos->column == 1)
		{
			lstcursor = lstcursor->next;
			pos->currentline--;
			go_upright(pos);
		}
		else
		{
			ft_putstr(tgetstr("le", NULL));
			lstcursor = lstcursor->next;
			stalk_cursor(pos);
		}
	}
	return (lstcursor);
}

t_node		*right(t_node *lstcursor, char buffer[], t_pos *pos)
{
	if (ARROW_RIGHT && lstcursor->prev)
	{
		if (pos->column == pos->termsize.ws_col)
		{
			lstcursor = lstcursor->prev;
			pos->currentline++;
			go_downleft(pos);
		}
		else
		{
			ft_putstr(tgetstr("nd", NULL));
			stalk_cursor(pos);
			lstcursor = lstcursor->prev;
		}
	}
	return (lstcursor);
}
