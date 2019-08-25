/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 10:31:14 by mbellaic          #+#    #+#             */
/*   Updated: 2019/08/25 18:15:17 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

/*
** !!
*/

char	*expand_double_schriek(t_pos *pos)
{
	if (pos->history->next && pos->history->next->next)
		return (pos->history->next->next->line);
	else
		return (NULL);
}

/*
** !string
*/

char	*expand_schriek_word(t_pos *pos, char *word)
{
	t_node *cursor;

	if (pos->history->next && pos->history->next->next)
		cursor = pos->history->next->next;
	else
		cursor = NULL;
	while (cursor)
	{
		if (ft_strncmp(cursor->line, word, ft_strlen(word)) == 0)
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

	i = 1;
	if (pos->history->next && pos->history->next->next)
		cursor = pos->history->next->next;
	else
		cursor = NULL;
	while(cursor && cursor->next)
		cursor = cursor->next;
	while (cursor && cursor->prev)
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

	i = 1;
	if (pos->history->next && pos->history->next->next)
		cursor = pos->history->next->next;
	else
		cursor = NULL;
	
	while (cursor)
	{
		if (i == number)
			return (cursor->line);
		i++;
		cursor = cursor->next;
	}
	return (NULL);
}
