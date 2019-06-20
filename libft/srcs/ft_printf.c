/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleduc <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 19:19:49 by aleduc            #+#    #+#             */
/*   Updated: 2018/11/20 13:18:15 by skuppers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_printf		*init_pf_struct(void)
{
	t_printf *new;

	if (!(new = (t_printf*)malloc(sizeof(t_printf))))
		return (NULL);
	new->padding = 0;
	new->padding_v1 = 0;
	new->padding_v2 = 0;
	new->negative = 0;
	return (new);
}

static int			add_param_to_buffer(char *buffer, char *str,
		va_list args)
{
	int			i;
	t_printf	*pf;
	char		*charset;

	i = 0;
	charset = "%dsc*-.";
	pf = init_pf_struct();
	while (ft_strchr(charset, str[i]) && str[i] != '\0')
	{
		if (str[i] == '-')
			i += set_struct_negative(pf);
		else if (str[i] == '*' && str[i + 1] == '.' && str[i + 2] == '*')
			i += set_struct_padding(pf, va_arg(args, int), va_arg(args, int));
		else if (str[i] == CHAR)
			i += add_char_to_buffer(buffer, va_arg(args, int), pf);
		else if (str[i] == STRING)
			i += add_string_to_buffer(buffer, va_arg(args, char*), pf);
		else if (str[i] == INT)
			i += add_int_to_buffer(buffer, va_arg(args, int), pf);
		else
			i++;
	}
	free(pf);
	ft_strdel(&str);
	return (i);
}

static void			print_buffer_and_clean(char *buff, int fd)
{
	ft_putstr_fd(buff, fd);
	ft_bzero(buff, BUFFER_SIZE);
}

void				ft_printf_fd(int fd, const char *str, ...)
{
	va_list		args;
	char		*buffer;
	size_t		buffer_limit;

	va_start(args, str);
	buffer_limit = BUFFER_SIZE - (BUFFER_SIZE / 3);
	buffer = ft_strnew(BUFFER_SIZE + 1);
	while (*str != '\0')
	{
		if (*str == '%')
			str += add_param_to_buffer(buffer, ft_strdup(str), args) - 1;
		else if (*str != '\\' && *str != '\t')
			ft_strncat(buffer, str, 1);
		if (ft_strlen(buffer) > buffer_limit)
			print_buffer_and_clean(buffer, fd);
		str++;
	}
	print_buffer_and_clean(buffer, fd);
	ft_strdel(&buffer);
	va_end(args);
}

void				ft_printf_va(int fd, const char *str, va_list args)
{
	char		*buffer;
	size_t		buffer_limit;

	buffer_limit = BUFFER_SIZE - (BUFFER_SIZE / 3);
	buffer = ft_strnew(BUFFER_SIZE + 1);
	while (*str != '\0')
	{
		if (*str == '%')
			str += add_param_to_buffer(buffer, ft_strdup(str), args) - 1;
		else if (*str != '\\' && *str != '\t')
			ft_strncat(buffer, str, 1);
		if (ft_strlen(buffer) > buffer_limit)
			print_buffer_and_clean(buffer, fd);
		str++;
	}
	print_buffer_and_clean(buffer, fd);
	ft_strdel(&buffer);
}
