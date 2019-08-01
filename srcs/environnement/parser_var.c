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

int			apply_parser_var(char *value, char **dst, int *i, int last)
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
	len = ft_strlen(*value);
	dst = ft_strdup("");
	while (++i < len)
	{
		expand = manage_is_quote((*value)[i], expand);
		if (expand <= 0 && ((*value)[i] == '$'
			|| ((*value)[i] == '~' && is_expand_tild(*value, i, expand))))
			last = apply_parser_var(*value, &dst, &i, last);
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
