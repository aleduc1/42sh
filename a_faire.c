/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 01:21:40 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/01 01:33:22 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	int	out;

	printf("coucou\n");
	out = dup(STDOUT_FILENO);
	dup2(2, STDOUT_FILENO);
	close(2);
	dprintf(STDOUT_FILENO, "toi\n");
	dup2(out, STDOUT_FILENO);
//	close(2);
//	close(out);
	printf("la\n");
	return (0);
}
