/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 10:31:14 by mbellaic          #+#    #+#             */
/*   Updated: 2019/08/24 19:28:45 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

/*
** !!
*/

char	*expand_double_schriek(t_pos *pos)
{
	ft_printf("Double Schriek\n");
	if (pos->history->next)
	{
		return (pos->history->next->line);
	}
	else
		return (NULL);
}

/*
** !string
*/

char	*expand_schriek_word(t_pos *pos, char *word)
{
	t_node *cursor;

	ft_printf("Schriek Word\n");
	cursor = pos->history->next;
	while (cursor)
	{
		if (ft_strncmp(cursor->line, word, ft_strlen(word)))
			return (cursor->line);
		cursor = cursor->next;
	}
	return (NULL);
}

/*
** !12
*/

char	*expand_schriek_number(t_pos *pos, int number)
{
	t_node *cursor;
	int i;

	ft_printf("Schriek Number\n");
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

/*
** !-12
*/

char	*expand_schriek_less(t_pos *pos, int number)
{
	t_node *cursor;
	int i;

	ft_printf("Schriek Less\n");
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
