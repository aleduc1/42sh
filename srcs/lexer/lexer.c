/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:21:29 by aleduc            #+#    #+#             */
/*   Updated: 2019/08/27 10:02:26 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int		handle_backslash(int *i, char **input)
{
	(*i)++;
	if ((*input)[(*i)] && (*input)[(*i) + 1] && (*input)[(*i) + 2])
		(*i)++;
	return (1);
}

int		lexer_begin_loop(char *input, int *i, int last_t)
{
	int		to_check;

	to_check = 0;
	if (ft_isreallyspace(input[*i]) && (*i != last_t))
		to_check = 1;
	else if (input[*i] == '\\' && (*i != last_t))
		to_check = 1;
	else if (input[*i] == '\\')
		to_check = handle_backslash(i, &input);
	return (to_check);
}

void	reading_loop(char *input, t_lex **lex, t_token **tok, t_tab_type **t)
{
	int		i;
	int		last_t;
	int		to_check;

	last_t = init_variables(tok, &to_check, &i, t);
	while (input[i])
	{
		i = handle_whitespace(input, i, lex);
		last_t = i;
		while (input[i] && !(to_check))
		{
			if ((to_check = lexer_begin_loop(input, &i, last_t)))
				;
			else if (input[i] == '\"' || input[i] == '\'')
				to_check = handle_string_case(&i, &last_t, &input, tok);
			else if (is_in_tab(t, input[i]))
				to_check = handle_classic_case(&i, &last_t, &input);
			else
				handle_word_case(&i, &input, &last_t, &to_check);
		}
		if (to_check && !(*tok))
			(*tok) = check_type(t, input, last_t, i);
		if (to_check)
			to_check = handle_tok(tok, lex);
	}
}

void	reading_input(char *input, t_lex **lex)
{
	t_token		*tok;
	t_tab_type	*tab_of_type;

	reading_loop(input, lex, &tok, &tab_of_type);
	ft_memdel((void **)&tab_of_type);
}

t_lex	*lexer(char *input)
{
	t_lex	*lex;

	lex = NULL;
	reading_input(input, &lex);
	lex = add_delim(&lex);
	handle_quotes(&lex);
	simple_command(&lex);
	if (handle_redir(&lex))
	{
		clean_lex(&lex);
		ft_error(0);
		return (NULL);
	}
	remove_space_token(&lex);
	return (lex);
}
