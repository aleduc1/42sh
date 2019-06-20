/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 10:31:02 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 09:57:34 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	copy_value(char *src, char **dst, int start, int end)
{
	char	*tmp;
	char	*stock;

	stock = ft_strsub(src, start, end - start);
	tmp = ft_strjoin(*dst, stock);
	ft_strdel(&(*dst));
	ft_strdel(&stock);
	(*dst) = tmp ? tmp : NULL;
}

char		*search_var(char *src)
{
	int		i;
	int		len;
	int		last;
	char	*dst;

	i = -1;
	last = 0;
	len = ft_strlen(src);
	dst = ft_strdup("");
	while (++i < len)
	{
		if (src[i] == '$' || (i == 0 && src[i] == '~'))
		{
			if (i != last)
				copy_value(src, &dst, last, i);
			if (src[i] == '~' && i == 0)
				i = manage_home(src, &dst, i) - 1;
			else
				i = apply_rules(src, &dst, i);
			last = i + 1;
		}
	}
	if (i != last)
		copy_value(src, &dst, last, i);
	return (dst);
}

void		parser_var(char ***value)
{
	char	*tmp;
	int		index;

	index = -1;
	while ((*value)[++index])
	{
		if ((*value)[index][0] == '"')
			index = ft_apply_dquote(&(*value), index);
		if (index == -1)
			break ;
		if ((*value)[index][0] == '\'')
			ft_remove_quote(&((*value)[index]));
		else if (ft_strchr_exist((*value)[index], '$')
				|| (*value)[index][0] == '~')
		{
			tmp = search_var((*value)[index]);
			ft_strdel(&((*value)[index]));
			(*value)[index] = tmp ? tmp : NULL;
		}
	}
}

void		parser_var_simple(char **value)
{
	char	*tmp;
	int		index;

	index = -1;
	if (*value)
	{
		if ((*value)[0] == '"')
			ft_remove_quote(value);
		if ((*value)[0] == '\'')
			ft_remove_quote(value);
		else if (ft_strchr_exist(*value, '$')
				|| (*value)[0] == '~')
		{
			tmp = search_var(*value);
			ft_strdel(value);
			(*value) = tmp ? tmp : NULL;
		}
	}
}
