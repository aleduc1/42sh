/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formats_parameter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 11:12:26 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:33:34 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"


static int	is_special_variable(char *str)
{
	if ((!str) || ft_strlen(str) != 1)
		return (0);
	if (str[0] == '?' || str[0] == '0' || str[0] == '-' || str[0] == '#')
		return (1);
	return (0);
}

int		check_format_variable(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (is_special_variable(str));
	return (1);
}

/*
** ft_strlen(parameter)
*/

char	*parameter_hash_first(char *parameter)
{
	int		len;
	char	*src;

	len = 0;
	if (check_format_variable(parameter) == 0)
	{
		src = ft_strjoin("#", parameter);
		display_error_expansion(src);
		ft_strdel(&src);
		return (NULL);
	}
	src = value_line_path(parameter, 0);
	if (src)
	{
		len = ft_strlen(src);
		ft_strdel(&src);
	}
	src = ft_itoa(len);
	return (src);
}

/*
** diff entre parameter et word affiche parameter. ex: salut#ok > salut ;
** salut#sal > ut ; salut#salut > ; salut#salutoi > salut
*/

char	*return_good_dst(int index, char *stock, char *parameter)
{
	char	*dst;
	int		i;

	i = -1;
	dst = NULL;
	if ((!stock) || ft_strlen(parameter) < ft_strlen(stock))
		i = 0;
	else
		while (parameter[++i] && stock[i])
			if (parameter[i] != stock[i])
			{
				i = 0;
				break ;
			}
	dst = ft_strsub(parameter, i, ft_strlen(parameter) - i);
	return (dst);
}

char	*parameter_hash_end(char *value)
{
	char	*parameter;
	char	*dst;
	char	*stock;
	int		index;

	index = ft_chr_index(value, '#');
	stock = ft_strsub(value, 0, index);
	if (check_format_variable(stock) == 0)
	{
		ft_strdel(&stock);
		display_error_expansion(value);
		return (NULL);
	}
	parameter = value_line_path(stock, 0);
	ft_strdel(&stock);
	if (!parameter)
		return (NULL);
	index += value[index + 1] && value[index + 1] == '#' ? 2 : 1;
	stock = value[index] ? search_var(value + index) : NULL;
	dst = return_good_dst(index, stock, parameter);
	ft_strdel(&parameter);
	ft_strdel(&stock);
	return (dst);
}

/*
** comme # mais inverse. ex: salut#sal > salut ;
** salut#ut > sal ; salut#salut > ; salut#salutoi > salut
*/

int		assign_value_parameter_percents(int *index, char **stock, char *value,
	char *parameter)
{
	ft_strdel(stock);
	(*index) += value[(*index) + 1] && value[(*index) + 1] == '%' ? 2 : 1;
	(*stock) = value[(*index)] ? search_var(value + (*index)) : NULL;
	(*index) = ft_strlen(parameter);
	return (ft_strlen(*stock));
}

char	*parameter_percents(char *value)
{
	char	*parameter;
	char	*dst;
	char	*stock;
	int		index;
	int		i;

	index = ft_chr_index(value, '%');
	stock = ft_strsub(value, 0, index);
	if (check_format_variable(stock) == 0)
	{
		ft_strdel(&stock);
		display_error_expansion(value);
		return (NULL);
	}
	parameter = value_line_path(stock, 0);
	if (!parameter)
		return (NULL);
	i = assign_value_parameter_percents(&index, &stock, value, parameter);
	if (i <= index)
		while (--index > -1 && --i > -1 && parameter[index] && stock[i])
			if (parameter[index] != stock[i])
			{
				i = 0;
				break ;
			}
	dst = ft_strsub(parameter, 0, index + 1);
	ft_strdel(&parameter);
	ft_strdel(&stock);
	return (dst);
}
