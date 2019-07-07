/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 10:31:02 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/07 18:17:49 by sbelondr         ###   ########.fr       */
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

int			manage_is_quote(char c, int expand)
{
	if (c == '\'')
	{
		if (expand == 0)
			return (expand + 1);
		if (expand > 0)
			return (expand - 1);
	}
	if (c == '"')
	{
		if (expand == 0)
			return (expand - 1);
		if (expand < 0)
			return (expand + 1);
	}
	return (expand);
}

int			expand_authorize(char c)
{
	if (c == '&' || c == '|' || c == ';' || c == '/')
		return (1);
	return (0);
}

int			is_expand_tild(char *value, int index, int expand)
{
	if (expand != 0)
		return (0);
	if ((!value[index - 1])
		|| ft_isspace(value[index - 1]) || expand_authorize(value[index - 1]))
		if ((!value[index + 1])
			|| ft_isspace(value[index + 1])
			|| expand_authorize(value[index + 1]))
			return (1);
	return (0);
}


/*
** utiliser ft_strsplit_commands(char *str) pour split sur ; et &
** verif et supression avec ft_arraydelline
** puis creer une fonction pour recoller les valeurs
*/

void		parser_var_test(char **value)
{
	int		i;
	int		expand;
	int		len;
	int		last;
	char	*dst;

	i = -1;
	last = 0;
	expand = 0;
	len = ft_strlen(*value);
	dst = ft_strdup("");
	while (++i < len)
	{
		expand = manage_is_quote((*value)[i], expand);
		if (expand <= 0 && ((*value)[i] == '$'
			|| ((*value)[i] == '~' && is_expand_tild(*value, i, expand))))
		{
			if (i != last)
				copy_value(*value, &dst, last, i);
			if ((*value)[i] == '~')
				i = manage_home(*value, &dst, i) - 1;
			else if ((*value)[i] == '$')
				i = apply_rules(*value, &dst, i);
			last = i + 1;
		}
	}
	if (i != last)
		copy_value(*value, &dst, last, i);
	ft_strdel(value);
	*value = dst;
}
