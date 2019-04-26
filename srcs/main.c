/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/25 19:01:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line lines[] = {
	{ .a = { 0, -25 },  .b = { 250, 25 } , .type = L_WALL},
	{ .a = { 250, 25 }, .b = { 500, 75 } , .type = L_WALL},
	{ .a = { 500, 75 }, .b = { 500, 300 }, .type = L_WALL},
	{ .a = {500, 300},  .b = { 800, 300 }, .type = L_WALL},
	{ .a = {800, 300},  .b = { 800, 350 }, .type = L_WALL},
	{ .a = {800, 350},  .b = { 800, 550 }, .type = L_WALL},
	{ .a = {800, 550},  .b = {0, 550 }   , .type = L_WALL},
	{ .a = {0, 550},    .b = {0, -25}    , .type = L_WALL},
	{ .a = { 200, 260}, .b = { 200, 160} , .type = L_WALL},
	{ .a = { 200, 160}, .b = { 100, 160} , .type = L_WALL},
	{ .a = { 100, 160}, .b = { 100, 260} , .type = L_WALL},
	{ .a = { 100, 260}, .b = { 200, 260} , .type = L_WALL},

	{ .a = { 0, -25 },  .b = { 250, 25 } , .type = L_FLOOR},
	{ .a = { 250, 25 }, .b = { 500, 75 } , .type = L_FLOOR},
	{ .a = { 500, 75 }, .b = { 500, 300 }, .type = L_FLOOR},
	{ .a = {500, 300},  .b = { 800, 300 }, .type = L_FLOOR},
	{ .a = {800, 300},  .b = { 800, 350 }, .type = L_FLOOR},
	{ .a = {800, 350},  .b = { 800, 550 }, .type = L_FLOOR},
	{ .a = {800, 550},  .b = {0, 550 }   , .type = L_FLOOR},
	{ .a = {0, 550},    .b = {0, -25}    , .type = L_FLOOR}
};

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	t_line_list *lst = NULL;
	int count = sizeof(lines) / sizeof(t_line);
	for (int i = 0; i < count; i++)
		append_list(&lst, (t_line){ (t_vec2){lines[i].a.x / 100, lines[i].a.y / 100}, (t_vec2){lines[i].b.x / 100, lines[i].b.y / 100}, lines[i].type});
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
