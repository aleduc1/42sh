/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_replace_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 21:26:38 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/25 23:14:08 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;

int					hp_is_reserved_word(char *str)
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

void				ft_delrepalias(t_repalias *del)
{
	if (!del)
		return ;
	ft_delrepalias(&(*del->next));
	ft_strdel(&(del->value));
	ft_strdel(&(del->key));
	ft_memdel((void **)&del);
}

char				*alias_rep_getfirstword(char *str)
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

char				*replace_alias(char *inpu, char *str, char *fir, char *val)
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
			inpu[i++] = str[j++];
	}
	inpu[len] = '\0';
	return (inpu);
}
