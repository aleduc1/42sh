/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_a_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/05 16:37:02 by aleduc            #+#    #+#             */
/*   Updated: 2019/05/09 07:34:33 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int		is_a_redirect(t_type t)
{
	if (t == GREAT || t == GREATAMP || t == AMPGREAT || t == DGREAT
		|| t == LESS || t == LESSAMP || t == AMPLESS || t == DLESS
		|| t == LESSAMPHYPH || t == GREATAMPHYPH)
		return (1);
	return (0);
}
