/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 15:10:40 by mbellaic          #+#    #+#             */
/*   Updated: 2019/08/01 14:44:51 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "job.h"

int		solo_tree(t_ast *node, t_pos *pos)
{
	if (node->token->type == CMD)
	{
		run_cmd(node->token, pos, 0);
		return (1);
	}
	if (node->token->type == AMP && node->l->token->type == CMD &&!node->r)
	{
		manage_id_job(1);
		run_cmd(node->l->token, pos, 1);
		return (1);
	}
	else
		return (-1);
}

void		background_case(t_ast *node, t_pos *pos, int *bg)
{
	if (node->token->type == AMP)
	{
		*bg = *bg+1;
		if (node->l->token->type == CMD && node->r \
							&& node->r->token->type == CMD) // OK
		{
			// ft_putendl(ft_itoa(*bg));
			run_cmd(node->l->token, pos, *bg);
			// if (*bg > 1)
			// 	ft_putendl(ft_itoa(*bg));
			// else
			// 	ft_putendl("0");

			// *bg = *bg-1;
			// ft_putendl(ft_itoa(*bg-1));
			run_cmd(node->r->token, pos, *bg-1);
		}
		if (node->l->token->type == CMD && node->r \
							&& node->r->token->type != CMD) // OK`
			run_cmd(node->l->token, pos, *bg-1);
	}
}

void	scolon_case(t_ast *node, t_pos *pos, int bg)
{
	if (node->token->type == SCOLON)
	{
		if (node->l->token->type == CMD && node->r->token->type == CMD)
		{
			// ft_putendl(ft_itoa(bg));
			run_cmd(node->l->token, pos, bg);
			// ft_putendl(ft_itoa(bg));
			run_cmd(node->r->token, pos, bg);
		}
		if (node->l->token->type == CMD && node->r->token->type != CMD)
			run_cmd(node->l->token, pos, bg);
	}
}

void	spipe_case(t_ast *node, t_pos *pos, int bg)
{
	static int state;

	if (node->token->type == SPIPE)
	{
		// ft_putendl(ft_itoa(bg));
		if (state == 0)
			run_pipe(node->l->token, pos, state++, bg);
		else
			run_pipe(node->l->token, pos, 1, bg);
		if (node->r->token->type == CMD)
		{
			run_pipe(node->r->token, pos, 2, bg);
			state = 0;
		}
	}
}

void	dpipe_case(t_ast *node, t_pos *pos, int bg)
{
	if (node->token->type == DPIPE)
	{
		if (node->l->token->type == CMD && node->r->token->type == CMD)
		{
			// ft_putendl(ft_itoa(bg));
			run_cmd(node->l->token, pos, bg);
			// printf("CHECKLAST:%d\n", check_last());
			if (check_last() != 0)
				run_cmd(node->r->token, pos, bg);
		}
		if (node->l->token->type == CMD && node->r->token->type != CMD)
			run_cmd(node->l->token, pos, bg);
	}
}

void	damp_case(t_ast *node, t_pos *pos, int bg)
{
	if (node->token->type == DAMP)
	{
		if (node->l->token->type == CMD && node->r->token->type == CMD)
		{
			// ft_putendl(ft_itoa(bg));
			run_cmd(node->l->token, pos, bg);
			// printf("CHECKL LAST:%d\n", check_last());
			if (check_last() == 0)
				run_cmd(node->r->token, pos, bg);
		}
		if (node->l->token->type == CMD && node->r->token->type != CMD)
			run_cmd(node->l->token, pos, bg);
	}
}

void	edge_case(t_ast *node, t_pos *pos, int bg)
{
	if (node->token->type == SCOLON || node->token->type == AMP)
		if (node->l->token->type != CMD && node->r\
							&& node->r->token->type == CMD)
			run_cmd(node->r->token, pos, bg);
	if (node->token->type == DPIPE)
		if (node->l->token->type != CMD && node->r->token->type == CMD)
			if (check_last() != 0)
				run_cmd(node->r->token, pos, bg);
	if (node->token->type == DAMP)
		if (node->l->token->type != CMD && node->r->token->type == CMD)
			if (check_last() == 0)
				run_cmd(node->r->token, pos, bg);
}

int		interpreter(t_ast *node, t_pos *pos, int background)
{
	static int bg;

	(void)background;
	if (!node)
		return (-1);
	background_case(node, pos, &bg);
	scolon_case(node, pos, bg);
	dpipe_case(node, pos, bg);
	damp_case(node, pos, bg);
	spipe_case(node, pos, bg);
	if (bg != 0)
		manage_id_job(bg);
	interpreter(node->l, pos, bg);
	if (node->token->type == AMP)
		bg--;
	interpreter(node->r, pos, bg);
	edge_case(node, pos, bg);
	return (0);
}

// FIX SEGFAULT WHEN NOTHING AFTER '&' MIGHT BE FUCKING SOMETHING ELSE, CAREFUL!
