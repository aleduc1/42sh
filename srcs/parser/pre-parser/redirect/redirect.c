/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 11:00:04 by aleduc            #+#    #+#             */
/*   Updated: 2019/05/09 07:33:48 by aleduc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	ft_default(t_redir **redir_info)
{
	t_redir	*ptr;
	t_type	t;

	ptr = *redir_info;
	t = ptr->type;
	if (t == GREAT || t == LESSAMP || t == DGREAT || t == GREATAMPHYPH \
			|| t == LESSAMPHYPH)
		ptr->src_fd[0] = ft_strdup("1");
	else if (t == GREATAMP || t == AMPGREAT || t == AMPLESS)
	{
		ptr->src_fd[0] = ft_strdup("1");
		ptr->src_fd[1] = ft_strdup("2");
	}
	else if (t == LESS || t == DLESS)
		ptr->src_fd[0] = ft_strdup("0");
}

int		handle_needed_redir(t_lex **command_node, t_lex **redir_node)
{
	int		ret;

	ret = 0;
	if ((*redir_node)->token->type == GREAT)
		ret = handle_great(command_node);
	else if ((*redir_node)->token->type == GREATAMP)
		ret = handle_great_and(command_node);
	else if ((*redir_node)->token->type == AMPGREAT)
		ret = handle_and_great(command_node);
	else if ((*redir_node)->token->type == DGREAT)
		ret = handle_append(command_node);
	else if ((*redir_node)->token->type == LESS)
		ret = handle_less(command_node);
	else if ((*redir_node)->token->type == LESSAMP)
		ret = handle_less_and(command_node);
	else if ((*redir_node)->token->type == AMPLESS)
		ret = handle_and_less(command_node);
	else if ((*redir_node)->token->type == DLESS)
		ret = handle_heredoc(command_node);
	else if ((*redir_node)->token->type == GREATAMPHYPH)
		ret = handle_great_hyph(command_node);
	else if ((*redir_node)->token->type == LESSAMPHYPH)
		ret = handle_less_hyph(command_node);
	return (ret);
}

int		cycle_redirect(t_lex **command_node)
{
	t_lex	*ptr;
	t_lex	*start;

	ptr = (*command_node)->token->command;
	start = ptr;
	while (ptr)
	{
		if (is_a_redirect(ptr->token->type))
		{
			if (handle_needed_redir(&start, &ptr))
				return (1);
			ptr = start;
		}
		else
			ptr = ptr->next;
	}
	return (0);
}

int		handle_redir(t_lex **lex)
{
	t_lex	*ptr;

	ptr = *lex;
	while (ptr)
	{
		if (ptr->token->type == CMD)
		{
			if (cycle_redirect(&ptr))
				return (1);
		}
		ptr = ptr->next;
	}
	return (0);
}
