/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 15:10:40 by mbellaic          #+#    #+#             */
/*   Updated: 2019/05/30 17:37:59 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int		solo_tree(t_ast *node, t_pos *pos)
{
	if (node->token->type == CMD)
	{
		run_cmd(node->token, pos);
		return (1);
	}
	else
		return (-1);
}

void	scolon_case(t_ast *node, t_pos *pos)
{
	if (node->token->type == SCOLON)
	{
		if (node->l->token->type == CMD && node->r->token->type == CMD)
		{
			run_cmd(node->l->token, pos);
			run_cmd(node->r->token, pos);
		}
		if (node->l->token->type == CMD && node->r->token->type != CMD)
			run_cmd(node->l->token, pos);
	}
}

void	spipe_case(t_ast *node, t_pos *pos)
{
	static int state;

    if (node->token->type == SPIPE)
    {
        if (state == 0)
            run_pipe(node->l->token, pos, state++);
        else
            run_pipe(node->l->token, pos, 1);
        if (node->r->token->type == CMD)
        {
            run_pipe(node->r->token, pos, 2);
            state = 0;
        }
    }
	/*
	if (node->token->type == SPIPE)
	{
		run_pipe(node->l->token, pos, 0);
		if (node->r->token->type == CMD)
			run_pipe(node->r->token, pos, 1);
	}*/
}

void	edge_case(t_ast *node, t_pos *pos)
{
	if (node->token->type == SCOLON)
		if (node->r->token->type == CMD && node->l->token->type != CMD)
			run_cmd(node->r->token, pos);
}

int		interpreter(t_ast *node, t_pos *pos)
{
	if (!node)
		return (-1);
	scolon_case(node, pos);
	spipe_case(node, pos);
	interpreter(node->l, pos);
	interpreter(node->r, pos);
	edge_case(node, pos);
	return (0);
}
