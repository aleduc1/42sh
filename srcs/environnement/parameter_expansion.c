/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 12:02:28 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:14:59 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

/*
** split :
** find arg
** apply ft avec tableau de pointeur
*/

static char		*(**init_array(void))(char*, char*)
{
	char	*(**conv)(char*, char*);
	int		i;

	conv = ft_memalloc(255 * sizeof(void*));
	i = -1;
	while (++i <= 254)
		conv[i] = 0;
	conv['-'] = &parameter_moins;
	conv['='] = &parameter_equals;
	conv['?'] = &parameter_interrogation;
	conv['+'] = &parameter_plus;
	return (conv);
}

char			*gest_expansion(char *key, char *value)
{
	char	*(**conv)(char*, char*);
	char	*final;

	conv = init_array();
	final = NULL;
	if (conv[(int)value[0]])
		final = (*conv[(int)value[0]])(key, value + 1);
	free(conv);
	conv = NULL;
	return (final);
}

int				is_other_expansion(char *tmp, char **dst)
{
	char	*stock;
	char	*stock_b;

	if (tmp[0] == '#')
		stock = parameter_hash_first(tmp + 1);
	else if (ft_chr_index(tmp, '#') > 0)
		stock = parameter_hash_end(tmp);
	else if (ft_chr_index(tmp, '%') > 0)
		stock = parameter_percents(tmp);
	else
		return (0);
	if (stock)
	{
		stock_b = ft_strjoin(*dst, stock);
		ft_strdel(dst);
		ft_strdel(&stock);
		*dst = stock_b;
	}
	return (1);
}

static void		display_error_expansion(char *src)
{
	ft_dprintf(STDERR_FILENO, "42sh: ${%s}: bad substitution\n", src);
	gest_return(-5);
}

void			manage_parameter_extension(char **dst, char *tmp, int i)
{
	char	*key;
	char	*tmp_b;

	key = ft_strsub(tmp, 0, i);
	if ((!key) || ft_strequ(key, ""))
	{
		display_error_expansion(tmp);
		return ;
	}
	tmp_b = gest_expansion(key, tmp + i + 1);
	ft_strdel(&key);
	key = ft_strjoin(*dst, tmp_b);
	ft_strdel(dst);
	ft_strdel(&tmp_b);
	(*dst) = key;
}

void			parameter_expansion(char *tmp, char **dst)
{
	int		i;

	if ((!tmp) || ft_strequ(tmp, ""))
	{
		display_error_expansion("");
		return ;
	}
	if ((i = ft_chr_index(tmp, ':')) < 0)
	{
		if (is_other_expansion(tmp, dst) == 0)
			modify_dst(tmp, dst);
	}
	else
		manage_parameter_extension(dst, tmp, i);
}
