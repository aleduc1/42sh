/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_replace_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 21:26:38 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/27 05:49:19 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern t_ht_hash	*g_alias_table;

t_repalias			*alias_rep_delalias(t_repalias *cur_alias)
{
	t_repalias	*curr;

	if (!cur_alias)
		return (NULL);
	curr = cur_alias;
	while (curr->next && curr->next->next)
		curr = curr->next;
	if (!curr->next)
	{
		curr->key = NULL;
		curr->value = NULL;
		free(curr);
		curr = NULL;
		return (NULL);
	}
	curr->next->key = NULL;
	curr->next->value = NULL;
	free(curr->next);
	curr->next = NULL;
	return (cur_alias);
}

t_repalias			*alias_rep_addalias(t_repalias *cur_alias, char *base,
										char *val)
{
	t_repalias	*new;
	t_repalias	*curr;

	if (!(new = (t_repalias *)malloc(sizeof(t_repalias))))
		return (cur_alias);
	new->key = base;
	new->value = val;
	new->next = NULL;
	if (!cur_alias)
		return (new);
	curr = cur_alias;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (cur_alias);
}
