/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_replace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 00:07:18 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/26 00:30:07 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int			hist_rep_exists(t_hist_rep *replace, t_pos *pos)
{
	int	num;

	if (ft_strequ(replace->base, "!!"))
		replace->value = expand_double_schriek(pos);
	else if (replace->base[1] == '\0')
		return (0);
	else if (ft_isstrnum(&(replace->base[1])))
	{
		num = ft_atoi(&(replace->base[1]));
		if (num < 0)
			replace->value = expand_schriek_less(pos, num * -1);
		else
			replace->value = expand_schriek_number(pos, num);
	}
	else
		replace->value = expand_schriek_word(pos, &(replace->base[1]));
	return (1);
}

char		*hist_rep_replace(t_hist_rep *replace, t_pos *pos)
{
	t_hist_rep	*curr;
	char		*new;

	curr = replace;
	new = ft_strnew(1);
	while (curr != NULL)
	{
		if (curr->isrep == 0 && curr->base != NULL)
			new = hist_rep_rep(new, curr->base);
		else
		{
			if (hist_rep_exists(curr, pos) && curr->value != NULL)
				new = hist_rep_rep(new, curr->value);
			else
				return (hist_rep_reperr(&(*new), curr));
		}
		curr = curr->next;
	}
	return (new);
}

t_hist_rep	*hist_rep_saveexp(char *input, int start, int i,
								t_hist_rep *replace)
{
	t_hist_rep		*new;
	t_hist_rep		*curr;
	int				j;

	if (start == i)
		return (replace);
	if (!(new = (t_hist_rep*)malloc(sizeof(t_hist_rep))))
		return (replace);
	new->isrep = 1;
	new->next = NULL;
	new->value = NULL;
	if (!(new->base = (char *)malloc(sizeof(char) * (i - start + 1))))
	{
		ft_memdel((void**)&new);
		return (replace);
	}
	j = 0;
	while (start < i)
	{
		new->base[j] = input[start];
		++j;
		++start;
	}
	new->base[j] = '\0';
	if (!(replace))
		return (new);
	curr = replace;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	return (replace);
}

t_hist_rep	*hist_rep_save(char *input, int start, int i, t_hist_rep *replace)
{
	t_hist_rep		*new;
	t_hist_rep		*curr;
	int				j;

	if (start == i)
		return (replace);
	if (!(new = (t_hist_rep*)malloc(sizeof(t_hist_rep))))
		return (replace);
	new->isrep = 0;
	new->next = NULL;
	new->value = NULL;
	if (!(new->base = (char *)malloc(sizeof(char) * (i - start + 1))))
	{
		ft_memdel((void**)&new);
		return (replace);
	}
	j = 0;
	while (start < i)
	{
		new->base[j] = input[start];
		++j;
		++start;
	}
	new->base[j] = '\0';
	if (!(replace))
		return (new);
	curr = replace;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	return (replace);
}

char		*history_replace(char *input, t_pos *pos)
{
	char		*new_input;
	t_hist_rep	*replace;
	int			i;
	int			start;
	int			expand;

	replace = NULL;
	start = 0;
	i = 0;
	expand = 0;
	if (!input)
		return (input);
	while (input[i])
	{
		expand = manage_is_quote(input, i, expand);
		if (input[i] == '!' && hist_rep_isvalid(&(input[i])) && expand == 0
			&& !(hist_rep_isbslashed(input, i - 1, expand)))
		{
			replace = hist_rep_save(input, start, i, replace);
			start = i;
			i += hist_rep_getexp(&(input[start]));
			replace = hist_rep_saveexp(input, start, i, replace);
			start = i;
		}
		else if (!input[i + 1] && replace != NULL)
		{
			replace = hist_rep_save(input, start, i + 1, replace);
			++i;
			start = i;
		}
		else
			++i;
	}
	if (!replace)
		return (input);
	new_input = hist_rep_replace(*(&replace), pos);
	hist_rep_delstruct(&(*replace));
	ft_strdel(&input);
	if (new_input)
		ft_printf("%s\n", new_input);
	return (new_input);
}
