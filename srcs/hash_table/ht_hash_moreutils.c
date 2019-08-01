/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_hash_moreutils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 10:11:07 by apruvost          #+#    #+#             */
/*   Updated: 2019/06/11 17:10:49 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_hash		g_hash_deleted = {NULL, NULL};

void		ht_hash_copy(t_ht_hash *ht, t_ht_hash *new_ht)
{
	int	i;
	t_hash		*item;

	i = 0;
	while (i < ht->size)
	{
		item = ht->hash[i];
		if (item != NULL && item != &g_hash_deleted)
			ht_hash_insert(new_ht, item->key, item->value);
		++i;
	}
}

void		ht_hash_resize(t_ht_hash *ht, const int base_size)
{
	t_ht_hash	*new_ht;
	int			tmp_size;
	t_hash		**items;

	if (base_size < HT_HASH_BASE_SIZE)
		return ;
	new_ht = ht_hash_new_sized(base_size);
	ht_hash_copy(ht, new_ht);
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;
	tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;
	items = ht->hash;
	ht->hash = new_ht->hash;
	new_ht->hash = items;
	ht_hash_del(new_ht);
}

void		ht_hash_delete(t_ht_hash *ht, const char *key)
{
	int		index;
	t_hash	*item;
	int		i;
	int		load;

	load = ht->count * 100 / ht->size;
	if (load < 10)
		ht_hash_resize_down(ht);
	index = ht_hash_get_hash(key, ht->size, 0);
	item = ht->hash[index];
	i = 1;
	while (item != NULL)
	{
		if (item != &g_hash_deleted)
		{
			if (ft_strcmp(item->key, key) == 0)
			{
				hash_del(item);
				ht->hash[index] = &g_hash_deleted;
			}
		}
		index = ht_hash_get_hash(key, ht->size, i);
		item = ht->hash[index];
		++i;
	}
	ht->count--;
}

char		*ht_hash_search(t_ht_hash *ht, const char *key)
{
	t_hash	*item;
	int		index;
	int		i;

	index = ht_hash_get_hash(key, ht->size, 0);
	item = ht->hash[index];
	i = 1;
	while (item != NULL)
	{
		if (item != &g_hash_deleted)
			if (ft_strcmp(item->key, key) == 0)
				return (item->value);
		index = ht_hash_get_hash(key, ht->size, i);
		item = ht->hash[index];
		++i;
	}
	return (NULL);
}

void		ht_hash_insert(t_ht_hash *ht, const char *key, char *value)
{
	t_hash	*item;
	int		index;
	t_hash	*cur_item;
	int		i;
	int		load;

	load = ht->count * 100 / ht->size;
	if (load > 70)
		ht_hash_resize_up(ht);
	item = hash_new_item(key, value);
	index = ht_hash_get_hash(item->key, ht->size, 0);
	cur_item = ht->hash[index];
	i = 1;
	while (cur_item != NULL)
	{
		if (cur_item != &g_hash_deleted)
		{
			if (ft_strcmp(cur_item->key, key) == 0)
			{
				hash_del(cur_item);
				ht->hash[index] = item;
				return ;
			}
		}
		index = ht_hash_get_hash(key, ht->size, i);
		cur_item = ht->hash[index];
		++i;
	}
	ht->hash[index] = item;
	++ht->count;
}
