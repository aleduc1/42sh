/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 15:17:51 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/19 22:26:16 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	hash_del(t_hash *hash)
{
	free(hash->key);
	hash->key = NULL;
	free(hash->value);
	hash->key = NULL;
	free(hash);
	hash = NULL;
}

t_hash	*hash_new_item(const char *k, const char *v)  // PROTECT MALLOC CAREFUL
{
	t_hash	*item;
	
	item = (t_hash *)malloc(sizeof(t_hash));
	item->key = ft_strdup(k);
	item->value = ft_strdup(v);
	return (item);
}
