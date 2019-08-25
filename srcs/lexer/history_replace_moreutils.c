/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_replace_moreutils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 23:40:05 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/26 00:29:10 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char		*hist_rep_rep(char *bin, char *value)
{
	char	*new;

	new = ft_strjoin(bin, value);
	ft_strdel(&bin);
	return (new);
}

char		*hist_rep_reperr(char *new, t_hist_rep *curr)
{
	ft_strdel(&new);
	ft_dprintf(2, "42sh: %s: event not found\n", curr->base);
	return (NULL);
}
