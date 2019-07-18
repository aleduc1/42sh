/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 10:31:02 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/08 00:11:24 by sbelondr         ###   ########.fr       */
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
	if (index - 1 < 0
		|| ft_isspace(value[index - 1]) || expand_authorize(value[index - 1]))
		if ((!value[index + 1])
			|| ft_isspace(value[index + 1])
			|| expand_authorize(value[index + 1]))
			return (1);
	return (0);
}

int			apply_parser_var_test(char *value, char **dst, int *i, int last)
{
	if ((*i) != last)
		copy_value(value, dst, last, (*i));
	if (value[(*i)] == '~')
		(*i) = manage_home(value, dst, (*i)) - 1;
	else if (value[(*i)] == '$')
		(*i) = apply_rules(value, dst, (*i));
	return ((*i) + 1);
}

void		parser_var_simple(char **value)
{
	int		i;
	int		expand;
	int		len;
	int		last;
	char	*dst;

	i = -1;
	last = 0;
	expand = 0;
	// ft_printf("value: [%s]\n", *value);
	len = ft_strlen(*value);
	dst = ft_strdup("");
	while (++i < len)
	{
		expand = manage_is_quote((*value)[i], expand);
		if (expand <= 0 && ((*value)[i] == '$'
			|| ((*value)[i] == '~' && is_expand_tild(*value, i, expand))))
			last = apply_parser_var_test(*value, &dst, &i, last);
	}
	if (i != last)
		copy_value(*value, &dst, last, i);
	ft_strdel(value);
	*value = dst;
	remove_quote_line(value);
}

void		parser_var(char ***value)
{
	int		i;

	i = -1;
	if ((!value) || (!*value))
		return ;
	while ((*value)[++i])
		parser_var_simple(&((*value)[i]));
}
