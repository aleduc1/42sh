/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 01:21:40 by sbelondr          #+#    #+#             */
/*   Updated: 2019/08/15 00:37:01 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int	main(void)
{
	int	out;
	int	test;

	test = open("test", O_RDWR | O_TRUNC);

	dprintf(STDOUT_FILENO, "coucou, %d\n", STDOUT_FILENO);
	
	out = dup(STDOUT_FILENO);
	
	dup2(test, STDOUT_FILENO);
	dprintf(STDOUT_FILENO, "toi %d\n", STDOUT_FILENO);
	
	dup2(out, STDOUT_FILENO);
	close(out);
	
	dprintf(STDOUT_FILENO, "la %d\n", STDOUT_FILENO);
	
	close(test);
	return (0);
}
