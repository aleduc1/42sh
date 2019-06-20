/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 16:02:34 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/27 13:23:25 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	ft_remove_quote(char **str)
{
	int		len;
	char	*tmp;

	len = ft_strlen(*str);
	tmp = ft_strsub(*str, 1, len - 2);
	ft_strdel(&(*str));
	(*str) = tmp ? tmp : ft_strdup("");
}

int		ft_apply_dquote(char ***value, int index)
{
	char	*tmp;
	char	*final;
	int		len;

	ft_remove_quote(&((*value)[index]));
	if (index == 0)
		return (index);
	len = ft_strlen((*value)[index - 1]);
	if (len == 0)
		return (index);
	if ((*value)[index - 1][len - 1] == '=' && ft_strequ((*value)[0], "env"))
	{
		tmp = ft_strdup((*value)[index]);
		final = ft_strjoin((*value)[index - 1], tmp);
		ft_strdel(&tmp);
		ft_strdel(&((*value)[index - 1]));
		(*value)[index - 1] = final;
		ft_arraydelline(&(*value), index);
		index--;
	}
	return (index);
}
