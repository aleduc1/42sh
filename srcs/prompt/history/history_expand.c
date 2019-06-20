/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 10:31:14 by mbellaic          #+#    #+#             */
/*   Updated: 2019/05/27 12:16:04 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

char	*expand_double_schriek(t_pos *pos)
{
	if (pos->history->next)
		return (pos->history->next->line);
	else
		return (NULL);
}

char	*expand_schriek_word(t_pos *pos, char *word)
{
	t_node *cursor;

	cursor = pos->history->next;
	while (cursor)
	{
		if (ft_strncmp(cursor->line, word, ft_strlen(word)))
			return (cursor->line);
		cursor = cursor->next;
	}
	return (NULL);
}

char	*expand_schriek_number(t_pos *pos, int number)
{
	t_node *cursor;
	int i;

	i = 1;
	cursor = pos->history;
	while(cursor->next)
		cursor = cursor->next;
	while (cursor->prev)
	{
		if (i == number)
			return (cursor->line);
		i++;
		cursor = cursor->prev;
	}
	return (NULL);
}

char	*expand_schriek_less(t_pos *pos, int number)
{
	t_node *cursor;
	int i;

	i = 1;
	cursor = pos->history->next;
	while (cursor)
	{
		if (i == number)
			return (cursor->line);
		i++;
		cursor = cursor->next;
	}
	return (NULL);
}
