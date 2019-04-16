/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/16 19:53:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line lines[] = {
	{ { 0, 150 }, { 500, 75 } },
	{ { 500, 75 }, { 500, 300 } },
	{ {500, 300}, { 800, 300 }},
	{ {800, 300}, { 800, 450 }},
	{ {800, 450}, {0, 450 }},
	{ {0, 450}, {0, 150}}
};

int		main(void)
{
	t_line_list *lst = NULL;
	int count = sizeof(lines) / sizeof(t_line);
	for (int i = 0; i < count; i++)
		append_list(&lst, lines[i]);
	t_node *n = create_node(lst);
	build_node(n);
	if (!n)
		return (-1);
	print_node(n);
	return (0);
}
