/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:21:37 by aleduc            #+#    #+#             */
/*   Updated: 2019/05/15 16:37:16 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	set_tab_types_2(t_tab_type **tab_of_types)
{
	(*tab_of_types)[8].str = " ";
	(*tab_of_types)[8].type = SPACE;
	(*tab_of_types)[9].str = "&>";
	(*tab_of_types)[9].type = AMPGREAT;
	(*tab_of_types)[10].str = "&<";
	(*tab_of_types)[10].type = AMPLESS;
	(*tab_of_types)[11].str = "<&-";
	(*tab_of_types)[11].type = LESSAMPHYPH;
	(*tab_of_types)[12].str = ">&-";
	(*tab_of_types)[12].type = GREATAMPHYPH;
	(*tab_of_types)[13].str = NULL;
	(*tab_of_types)[13].type = NUL;
}

void	set_tab_types(t_tab_type **tab_of_types)
{
	if (!((*tab_of_types) = (t_tab_type *)ft_memalloc(sizeof(t_tab_type) * 14)))
		return ;
	(*tab_of_types)[0].str = "|";
	(*tab_of_types)[0].type = SPIPE;
	(*tab_of_types)[1].str = ";";
	(*tab_of_types)[1].type = SCOLON;
	(*tab_of_types)[2].str = "<";
	(*tab_of_types)[2].type = LESS;
	(*tab_of_types)[3].str = "<<";
	(*tab_of_types)[3].type = DLESS;
	(*tab_of_types)[4].str = "<&";
	(*tab_of_types)[4].type = LESSAMP;
	(*tab_of_types)[5].str = ">";
	(*tab_of_types)[5].type = GREAT;
	(*tab_of_types)[6].str = ">>";
	(*tab_of_types)[6].type = DGREAT;
	(*tab_of_types)[7].str = ">&";
	(*tab_of_types)[7].type = GREATAMP;
	set_tab_types_2(tab_of_types);
}
