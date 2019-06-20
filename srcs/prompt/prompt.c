/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:13:21 by aleduc            #+#    #+#             */
/*   Updated: 2019/06/10 22:53:27 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "env.h"

void			init_prompt(t_pos *pos)
{
	pos->history = NULL;
	dpush(&pos->history, 'X');
	pos->historysum = 0;
	pos->historycount = 0;
	file_to_history(pos);
	pos->clipboard = NULL;
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
	pos->multiline = 0;
	pos->stop = 0;
}

int				check_integrity(t_node *input, t_multi **multi, t_pos *pos,\
													t_integrity *count)
{
	t_node		*cursor;
	t_multi		*lstcursor;

	cursor = input;
	lstcursor = NULL;
	while (cursor->next != NULL)
		cursor = cursor->next;
	key_occurence(cursor, count);
	if ((count->dquote % 2) != 0)
	{
		pos->multiline = 1;
		dquote(lstcursor, multi, pos);
		return (-1);
	}
	if ((count->quote % 2) != 0)
	{
		pos->multiline = 1;
		quote(lstcursor, multi, pos);
		return (-1);
	}
	return (1);
}

t_node			*read_input(t_node **input, t_pos *pos)
{
	char		buffer[4096];
	t_node		*lstcursor;

	lstcursor = *input;
	reset_multi(pos);
	ft_bzero(buffer, 4096);
	ft_putstr(tgetstr("im", NULL));
	while (read(STDIN_FILENO, &buffer, 4095) < 4095 && !ENTER)
	{
		lstcursor = editline(pos, lstcursor, input, buffer);
		stalk_cursor(pos);
		ft_bzero(buffer, 4096);
		if (pos->stop == 1)
			return (*input);
	}
	travel_to_last(lstcursor, pos);
	ft_putstr("\n");
	return (*input);
}

char			*check_prompt(char *inputstr, t_multi **multi)
{
	if (getenv("TERM") != NULL)
	{
		raw_term_mode();
		print_prompt();
		multi_push(multi);
		(*multi)->input = NULL;
		dpush(&(*multi)->input, ' ');
		return ((char *)-1);
	}
	else
	{
		ft_putstr("\n\033[31m[DUMB_MODE] &> \033[0m");
		if ((get_next_line(STDIN_FILENO, &inputstr) != -1)\
			&& !ft_strequ(inputstr, ""))
			return (inputstr);
	}
	return (NULL);
}

char			*prompt(t_multi *multi, t_pos *pos)
{
	t_integrity	count;
	t_multi		*lstcursor;
	char		*inputstr;

	inputstr = NULL;
	if ((inputstr = check_prompt(inputstr, &multi)) != (char *)-1)
		return (inputstr);
	multi->input = read_input(&multi->input, pos);
	lstcursor = multi;
	if (multi->input)
	{
		ft_bzero(&count, sizeof(t_integrity));
		while (check_integrity(lstcursor->input, &multi, pos, &count) < 0 \
				&& pos->stop != 1)
			lstcursor = lstcursor->prev;
		lstcursor = multi;
		inputstr = lst_to_str(&multi, inputstr);
		if (inputstr && pos->stop != 1 && (ft_strequ(inputstr, history_no_double(pos))) == 0)
			inserthistory(pos->history, inputstr, pos);
	}
	ddellist(multi);
	reset_stop(&inputstr, pos, &count);
	pos->historycount = 0;
	default_term_mode();
	return (inputstr) ? inputstr : NULL;
}
