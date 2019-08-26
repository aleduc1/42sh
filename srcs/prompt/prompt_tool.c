/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 03:56:27 by mbellaic          #+#    #+#             */
/*   Updated: 2019/08/26 04:03:24 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "builtins.h"
#include "env.h"

void	line_to_history(char **inputstr, t_pos *pos)
{
	*inputstr = history_replace(*inputstr, pos);
	if (inputstr && pos->stop != 1 \
		&& (ft_strequ(*inputstr, history_no_double(pos))) == 0)
		inserthistory(pos->history, *inputstr, pos);
}
