/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morekeyhook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:18:42 by aleduc            #+#    #+#             */
/*   Updated: 2019/06/05 21:33:19 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "env.h"

t_node	*backwardmod(t_node *lstcursor, t_pos *pos)
{
	if (lstcursor->next && lstcursor->key != ' ' && lstcursor->next->key == ' ')
	{
		stalk_cursor(pos);
		if (pos->column == 1)
			go_upright(pos);
		else
			ft_putstr(tgetstr("le", NULL));
		lstcursor = lstcursor->next;
	}
	if (lstcursor->next && lstcursor->key == ' ')
	{
		while (lstcursor->next && lstcursor->key == ' ')
		{
			stalk_cursor(pos);
			if (pos->column == 1)
				go_upright(pos);
			else
				ft_putstr(tgetstr("le", NULL));
			lstcursor = lstcursor->next;
		}
	}
	return (lstcursor);
}

t_node	*backwardjump(t_node *lstcursor, char buffer[], t_pos *pos)
{
	if (PG_DOWN)
	{
		lstcursor = backwardmod(lstcursor, pos);
		while (lstcursor->next && lstcursor->next->key != ' ')
		{
			stalk_cursor(pos);
			if (pos->column == 1)
				go_upright(pos);
			else
				ft_putstr(tgetstr("le", NULL));
			lstcursor = lstcursor->next;
		}
	}
	return (lstcursor);
}

t_node	*forwardjump(t_node *lstcursor, char buffer[], t_pos *pos)
{
	if (PG_UP)
	{
		while (lstcursor->prev && lstcursor->key != ' ')
		{
			lstcursor = lstcursor->prev;
			stalk_cursor(pos);
			if (pos->column == pos->termsize.ws_col)
				go_downleft(pos);
			else
				ft_putstr(tgetstr("nd", NULL));
		}
		while (lstcursor->prev && lstcursor->key == ' ')
		{
			lstcursor = lstcursor->prev;
			if (pos->column == pos->termsize.ws_col)
				go_downleft(pos);
			else
				ft_putstr(tgetstr("nd", NULL));
		}
	}
	return (lstcursor);
}

t_node	*home_end(t_node *lstcursor, char buffer[], t_pos *pos)
{
	if (HOME)
		while (lstcursor->next != NULL)
		{
			if (pos->column == 1)
				go_upright(pos);
			else
				ft_putstr(tgetstr("le", NULL));
			lstcursor = lstcursor->next;
			stalk_cursor(pos);
		}
	if (END)
		while (lstcursor->prev != NULL)
		{
			if (pos->column == pos->termsize.ws_col)
				go_downleft(pos);
			else
				ft_putstr(tgetstr("nd", NULL));
			lstcursor = lstcursor->prev;
			stalk_cursor(pos);
		}
	return (lstcursor);
}

t_node	*ctrl_n_friends(t_node *lstcursor, t_node **input, char buffer[], \
																t_pos *pos)
{
	char	*search_result;
	int i;

	i = 0;
	if (CTRL_D && !(*input)->next && pos->multiline != 1)
	{
		history_file(pos->history);
		default_term_mode();
		get_env(1, NULL);
		exit(0);
	}
	if (CTRL_C)
	{
		while (((*input)->next) != NULL)
			ddel(input, *input);
		pos->stop = 1;
		pos->multiline = 0;
		travel_to_last(lstcursor, pos);
		ft_putchar('\n');
	}
	if (CTRL_R)
	{
		t_node *new;
		int		ret;

		ret = 0;
		new = NULL;
		if ((search_result = prompt_search(*input, pos, &ret)) != NULL)
		{
			dpush(&new, ' ');
			while (search_result && search_result[i])
				insert(new, search_result[i++]);
			while (*input)
				ddel(input, *input);
			*input = new;
		}
		if (ret == -1)
			while ((*input)->next)
				ddel(input, *input);
		lstcursor = *input;
		print_prompt();
		dprintlist(*input, 0);
		ft_putstr(tgetstr("le", NULL));
		ft_putstr(tgetstr("cd", NULL));
	}
	return (lstcursor);
}
