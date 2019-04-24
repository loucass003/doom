/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 01:52:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line lines[] = {
	{ { 0, -25 }, { 250, 25 } },
	{ { 250, 25 }, { 500, 75 } },
	{ { 500, 75 }, { 500, 300 } },
	{ {500, 300}, { 800, 300 }},
	{ {800, 300}, { 800, 350 }},
	{ {800, 350}, { 800, 550 }},
	{ {800, 550}, {0, 550 }},
	{ {0, 550}, {0, -25}},
	{{ 200, 260}, { 200, 160}},
	{{ 200, 160}, { 100, 160}},
	{{ 100, 160}, { 100, 260}},
	{{ 100, 260}, { 200, 260}}
};

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	t_line_list *lst = NULL;
	int count = sizeof(lines) / sizeof(t_line);
	for (int i = 0; i < count; i++)
		append_list(&lst, lines[i]);
	doom.first_node = create_node(lst);
	build_node(doom.first_node);
	if (!doom.first_node)
		return (-1);
	print_node(doom.first_node);
	if (!init_sdl(&doom))
	{
		ft_putstr("ERROR");
		return (-1);
	}
	doom.bsp = doom.first_node;
	if (!(doom.rendered_area = (uint8_t *)malloc(doom.screen.width)))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	return (0);
}
