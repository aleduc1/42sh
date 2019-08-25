/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_replace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:03:10 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/25 23:33:11 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;
static t_repalias	*g_alias;

static void			alias_change_new(char *val, char *firstword)
{
	t_repalias	*newa;

	if (!(newa = (t_repalias *)malloc(sizeof(t_repalias))))
	{
		ft_strdel(&firstword);
		return ;
	}
	newa->key = firstword;
	newa->value = ft_strdup(val);
	newa->next = g_alias;
	g_alias = newa;
}

static char			*alias_change(char *str)
{
	char		*firstword;
	char		*val;
	size_t		len;
	char		*input;

	if (!(firstword = alias_rep_getfirstword(str)))
		return (NULL);
	val = ht_hash_search(g_alias_table, firstword);
	len = ft_strlen(str) - ft_strlen(firstword) + ft_strlen(val);
	if (!(input = (char *)malloc(sizeof(char) * (len + 1))))
	{
		ft_strdel(&firstword);
		return (NULL);
	}
	if (!(input = replace_alias(input, str, firstword, val)))
	{
		ft_strdel(&firstword);
		return (NULL);
	}
	alias_change_new(val, &(*firstword));
	return (input);
}

static int			alias_isreplacebis(char *firstword)
{
	t_repalias	*curr;

	curr = g_alias;
	while (curr != NULL)
	{
		if (ft_strequ(firstword, curr->key))
		{
			ft_strdel(&firstword);
			return (0);
		}
		curr = curr->next;
	}
	ft_strdel(&firstword);
	return (1);
}

static int			alias_isreplace(char *str)
{
	char		*firstword;
	char		*val;

	if (!(firstword = alias_rep_getfirstword(str)))
		return (0);
	if (hp_is_reserved_word(firstword))
	{
		ft_strdel(&firstword);
		return (0);
	}
	val = ht_hash_search(g_alias_table, firstword);
	if (val == NULL)
	{
		ft_strdel(&firstword);
		return (0);
	}
	return (alias_isreplacebis(&(*firstword)));
}

char				*alias_replace(char *input)
{
	char		*str;
	char		*bin;

	str = input;
	g_alias = NULL;
	while (alias_isreplace(str))
	{
		bin = str;
		str = alias_change(str);
		ft_strdel(&bin);
	}
	ft_delrepalias(&(*g_alias));
	return (str);
}
