/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:19:28 by aleduc            #+#    #+#             */
/*   Updated: 2019/06/01 19:56:54 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "job.h"

void				welcome(void)
{



	ft_putstr("\033[2J\033[H");
	ft_putstr("\033[1;32m\n");
	ft_putstr("██╗  ██╗██████╗ ███████╗██╗  ██╗    \n");
	ft_putstr("██║  ██║╚════██╗██╔════╝██║  ██║    \n");
	ft_putstr("███████║ █████╔╝███████╗███████║    \n");
	ft_putstr("╚════██║██╔═══╝ ╚════██║██╔══██║    \n");
	ft_putstr("     ██║███████╗███████║██║  ██║    \n");
	ft_putstr("     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    \n");
	ft_putstr("\033[0m");
}

void				default_term_mode(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &term);
	ft_putstr(tgetstr("ei", NULL));
}

void				raw_term_mode(void)
{
	struct termios	term;
	t_shell			*s;

	s = get_shell();
	s->term = STDIN_FILENO;
	while (tcgetpgrp (s->term) != (s->pgid = getpgrp()))
        kill (- s->pgid, SIGTTIN);
	ign_signaux();
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO | ISIG);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	tgetent(NULL, getenv("TERM"));
	s->pgid = getpid();
	s->interactive = isatty(s->term);
	s->term_shell = term;
}
