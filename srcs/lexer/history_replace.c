/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_replace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 00:07:18 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/24 19:28:51 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void		hist_rep_delstruct(t_hist_rep *to_del)
{
	if (!to_del)
		return ;
	hist_rep_delstruct(to_del->next);
	ft_strdel(&(to_del->base));
	to_del->next = NULL;
	to_del->value = NULL;
	free(to_del);
	to_del = NULL;
	return ;
}

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
			replace->value = expand_schriek_less(pos, num);
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
	char		*bin;

	curr = replace;
	new = NULL;
	while (curr != NULL)
	{
		if (curr->isrep == 0)
		{
			bin = new;
			new = ft_strjoin(new, curr->base);
			ft_strdel(&bin);
		}
		else
		{
			if (hist_rep_exists(curr, pos))
			{
				bin = new;
				new = ft_strjoin(new, curr->base);
				ft_strdel(&bin);
			}
			else
				return (NULL);	//  		ERR
		}
		curr = curr->next;
	}
	return (new);
}

int			hist_rep_getexp(char *input)
{
	int		i;

	i = 1;
	while (input && input[i] && input[i] != ' ' && input[i] != '\t'
			&& input[i] != ';' && input[i] != '|' && input[i] != '&'
			&& input[i] != '(' && input[i] != ')' && input[i] != '<'
			&& input[i] != '>')
		++i;
	return (i);
}

t_hist_rep	*hist_rep_saveexp(char *input, int start, int i, t_hist_rep *replace)
{
	t_hist_rep		*new;
	t_hist_rep		*curr;

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
	while (start < i)
	{
		new->base[start] = input[start];
		++start;
	}
	new->base[i] = '\0';
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
	while (start < i)
	{
		new->base[start] = input[start];
		++start;
	}
	new->base[i] = '\0';
	if (!(replace))
		return (new);
	curr = replace;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	return (replace);
}

int			hist_rep_isvalid(char *input)
{
	if (input[0] != '!')
		return (0);
	else if (input[1] == ' ' || input[1] == '\t' || input[1] == '\0'
			|| input[1] == '=' || input[1] == '%' || input[1] == '\n')
		return (0);
	return (1);
}

char		*history_replace(char *input, t_pos *pos)
{
	char	*new_input;
	t_hist_rep	*replace;
	int		i;
	int		start;

	replace = NULL;
	start = 0;
	i = 0;
	if (!input)
		return (input);
	while (input[i])
	{
		if (input[i] == '!' && hist_rep_isvalid(&(input[i])))
		{
			replace = hist_rep_save(input, start, i, replace);
			start = i;
			i += hist_rep_getexp(&(input[start]));
			replace = hist_rep_saveexp(input, start, i, replace);
			start = i;
		}
		else
			++i;
	}
	if (!replace)
		return (input);
	new_input = hist_rep_replace(*(&replace), pos);
	hist_rep_delstruct(&(*replace));
	ft_printf("Input = %s\n", input);
	ft_printf("New input = %s\n", new_input);
	return (new_input);
}
