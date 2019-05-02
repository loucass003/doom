/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/02 14:33:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line	lines[] = {
	{ .a = { 0, -25 },  .b = { 250, 25 } },
	{ .a = { 250, 25 }, .b = { 500, 75 } },
	{ .a = { 500, 75 }, .b = { 500, 300 }},
	{ .a = {500, 300},  .b = { 800, 300 }},
	{ .a = {800, 300},  .b = { 800, 350 }},
	{ .a = {800, 350},  .b = { 800, 550 }},
	{ .a = {800, 550},  .b = {0, 550 }   },
	{ .a = {0, 550},    .b = {0, -25}    },
	{ .a = { 200, 260}, .b = { 200, 160} },
	{ .a = { 200, 160}, .b = { 100, 160} },
	{ .a = { 100, 160}, .b = { 100, 260} },
	{ .a = { 100, 260}, .b = { 200, 260} },
};

void	init_bsp(t_doom *doom)
{
	int count = sizeof(lines) / sizeof(t_line);
	t_arraylist *polygons = create_arraylist(count + 2);
	t_arraylist *vertices = create_arraylist(count);
	for (int i = 0; i < count; i++)
	{
		arraylist_appendm(&vertices, &(t_vec3){
			lines[i].a.x / 100., 0, lines[i].a.y / 100.}, sizeof(t_vec3));
	}
	arraylist_append(&polygons, create_polygon(vertices, P_FLOOR));
	t_arraylist *vertices_ceil = create_arraylist(count);
	for (int i = 0; i < count; i++)
	{
		arraylist_appendm(&vertices_ceil, &(t_vec3){
			lines[i].a.x / 100., 1, lines[i].a.y / 100.}, sizeof(t_vec3));
	}
	arraylist_append(&vertices_ceil, create_polygon(vertices, P_FLOOR));
	for (int i = 0; i < count; i++)
	{
		arraylist_append(&polygons, create_wall_polygon((t_line){ (t_vec2){lines[i].a.x / 100.,  -(lines[i].a.y / 100.)}, (t_vec2){lines[i].b.x / 100., -(lines[i].b.y / 100.)}}, 0, 1));
	}
	t_node *n = create_node(polygons);
	build_node(n);
	printf("graph TD\n");
	print_node(n);
	doom->bsp = n;
}

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	init_bsp(&doom);
	init_sdl(&doom);
	if (!(doom.rendered_area = (uint8_t *)malloc(doom.screen.width)))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	free_node(doom.bsp);
	return (0);
}
