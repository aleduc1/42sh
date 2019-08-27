/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 10:31:02 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/27 11:02:47 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int			ft_remove_element(char **line, int i)
{
	ft_strremove_char(line, i);
	if ((*line)[i] && (*line)[i] == '\\')
		++i;
	else if (!(*line)[i])
		return (1);
	else if ((*line)[i] == '\n')
	{
		ft_strremove_char(line, i);
		--i;
	}
	return (0);
}

void		removebackslash(char **line)
{
	int		i;
	int		expand;

	i = 0;
	expand = 0;
	if ((!line) || (!(*line)))
		return ;
	while ((*line) && (*line)[i])
	{
		expand = manage_is_quote((*line), i, expand);
		if ((*line)[i] == '\\' && expand > 0)
		{
			if (ft_remove_element(line, i))
				return ;
		}
		else if ((*line)[i] == '\\' && expand == 0)
		{
			if (ft_remove_element(line, i))
				return ;
		}
		else
			++i;
	}
}

void		remove_quote_line_first(char **line)
{
	int	i;
	int	expand;

	i = -1;
	expand = 0;
	if ((!line) || (!(*line)))
		return ;
	while ((*line)[++i])
	{
		expand = manage_is_quote((*line), i, expand);
		if ((((*line)[i] == '\'' && expand > 0)
			|| ((*line)[i] == '"' && expand < 0))
			&& (i - 1 < 0 || (*line)[i - 1] != '\\'))
		{
			ft_strremove_char(line, i);
			--i;
		}
		else if ((((*line)[i] == '\'' && expand == 0)
				|| ((*line)[i] == '"' && expand == 0))
				&& (i - 1 < 0 || (*line)[i - 1] != '\\'))
		{
			ft_strremove_char(line, i);
			--i;
		}
	}
}

void		remove_quote_line(char **line)
{
	remove_quote_line_first(line);
	removebackslash(line);
}

void		remove_quote(char ***value)
{
	int	i;

	i = -1;
	while ((*value)[++i])
	{
		remove_quote_line(&((*value)[i]));
		removebackslash(&((*value)[i]));
	}
}
