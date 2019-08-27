/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 10:31:02 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/27 08:28:28 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void		ft_strremove_char(char **src, int i)
{
	char	*tmp;
	char	*tmp_bis;

	if (i == 0)
	{
		tmp = ft_strsub(*src, 1, ft_strlen(*src) - 1);
		ft_strdel(src);
		(*src) = tmp;
		return ;
	}
	tmp = ft_strsub(*src, 0, i);
	tmp_bis = ((*src)[i + 1]) ? ft_strsub(*src, i + 1, ft_strlen(*src) - i)
			: ft_strdup("");
	ft_strdel(src);
	(*src) = ft_strjoin(tmp, tmp_bis);
	ft_strdel(&tmp);
	ft_strdel(&tmp_bis);
}

int			ft_remove_element(char **line, int i)
{
	ft_strremove_char(line, i);
	if ((*line)[i] && (*line)[i] == '\\')
		++i;
	else if (!(*line)[i])
		return (1);
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

void		remove_quote_line(char **line)
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
