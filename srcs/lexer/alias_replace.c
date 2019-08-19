/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_replace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:03:10 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/13 18:08:22 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;
static t_repalias	*alias;

static char			*replace_alias(char *inpu, char *str, char *fir, char *val)
{
	size_t	i;
	size_t	j;
	size_t	c;
	size_t	len;

	i = 0;
	j = 0;
	c = (size_t)ft_strstr(str, fir) - (size_t)str;
	len = ft_strlen(str) - ft_strlen(fir) + ft_strlen(val);
	while (i < len)
	{
		if (i == c)
		{
			inpu[i] = 0;
			inpu = ft_strcat(inpu, val);
			c = -1;
			i += ft_strlen(val);
			j += ft_strlen(fir);
		}
		else
		{
			inpu[i] = str[j];
			++i;
			++j;
		}
	}
	inpu[len] = '\0';
	return (inpu);
}

static char			*get_first_word(char *str)
{
	char	**words;
	char	*first;

	if (!str)
		return (NULL);
	words = NULL;
	if (!(words = ft_split_whitespaces(str)))
		return (NULL);
	if (words[0])
		first = ft_strdup(words[0]);
	else
		first = NULL;
	ft_strptrdel(&words);
	return (first);
}

static char			*alias_change(char *str)
{
	char		*firstword;
	char		*val;
	size_t		len;
	char		*input;
	t_repalias	*newa;

	if (!(firstword = get_first_word(str)))
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
	if (!(newa = (t_repalias *)malloc(sizeof(t_repalias))))
	{
		ft_strdel(&firstword);
		return (input);
	}
	newa->key = firstword;
	newa->value = ft_strdup(val);
	newa->next = alias;
	alias = newa;
	return (input);
}

static int			is_reserved_word(char *str)
{
	if (ft_strequ(str, "!") || ft_strequ(str, "do")
		|| ft_strequ(str, "esac") || ft_strequ(str, "in")
		|| ft_strequ(str, "{") || ft_strequ(str, "}")
		|| ft_strequ(str, "then") || ft_strequ(str, "elif")
		|| ft_strequ(str, "for") || ft_strequ(str, "until")
		|| ft_strequ(str, "case") || ft_strequ(str, "else")
		|| ft_strequ(str, "if") || ft_strequ(str, "while"))
		return (1);
	return (0);
}

static int			alias_isreplace(char *str)
{
	char		*firstword;
	char		*val;
	t_repalias	*curr;

	if (!(firstword = get_first_word(str)))
		return (0);
	if (is_reserved_word(firstword))
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
	curr = alias;
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

static void			ft_delrepalias(t_repalias *del)
{
	if (!del)
		return ;
	ft_delrepalias(&(*del->next));
	ft_strdel(&(del->value));
	ft_strdel(&(del->key));
	ft_memdel((void **)&del);
}

char				*alias_replace(char *input)
{
	char		*str;
	char		*bin;

	str = input;
	alias = NULL;
	while (alias_isreplace(str))
	{
		bin = str;
		str = alias_change(str);
		ft_strdel(&bin);
	}
	ft_delrepalias(&(*alias));
	return (str);
}
