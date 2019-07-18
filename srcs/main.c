/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/16 16:40:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line	lines[] = {
	{ .a = { 0, 150 },  .b = { 500, 75 } },
	{ .a = { 500, 75 }, .b = { 500, 300 } },
	{ .a = { 500, 300 }, .b = { 800, 300 }},
	{ .a = { 800, 300},  .b = { 800, 450 }},
	{ .a = { 800, 450},  .b = { 0, 450 }},
	{ .a = { 0, 450},  .b = { 0, 150 }},
};
t_line	lines2[] = {
	{ .a = { 0, 150 },  .b = { 500, 75 } },
	{ .a = { 500, 75 }, .b = { 500, 300 } },
	{ .a = { 500, 300 }, .b = { 800, 300 }},
	{ .a = { 800, 300},  .b = { 800, 450 }},
	{ .a = { 800, 450},  .b = { 0, 450 }},
	{ .a = { 0, 450},  .b = { 0, 150 }},
};


/*t_vec3 list[] = {
	{0, 0, 0},
	{0, 0, 50}, 
	{50, 0, 50},
	{50, 0, 100},
	{0, 0, 100},
	{0, 0, 150},
	{150, 0, 150},
	{150, 0, 100},
	{100, 0, 100},
	{100, 0, 50},
	{150, 0, 50},
	{150, 0, 0}
};*/

void	init_bsp(t_doom *doom)
{
	int count = sizeof(lines) / sizeof(t_line);
	int count2 = sizeof(lines2) / sizeof(t_line);
	t_polygons *polygons = create_polygons_array(count + 2);
	
	t_3dvertices *vertices = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices, (t_vec3){
			lines[i].a.x / 100, 0, lines[i].a.y / 100});
	}
	append_polygons_array(&polygons, create_polygon(vertices, P_FLOOR));
	
	t_3dvertices *vertices_ceil = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices_ceil, (t_vec3){
			lines[i].a.x / 100, 1.5, lines[i].a.y / 100});
	}
	append_polygons_array(&polygons, create_polygon(vertices_ceil, P_CEILING));
	
	for (int i = 0; i < count; i++)
	{
		append_polygons_array(&polygons, create_wall_polygon((t_line){ (t_vec2){lines[i].a.x / 100, (lines[i].a.y / 100)}, (t_vec2){lines[i].b.x / 100, (lines[i].b.y / 100)}}, 0, 1.5));
	}

	/* int count = sizeof(list) / sizeof(t_vec3);
	t_polygons *polygons = create_polygons_array(count);
	for (int i = 0; i < count; i++)
	{
		int next = (i + 1) % count;
		append_polygons_array(&polygons, create_wall_polygon((t_line){ {list[i].x / 100, list[i].z / 100}, {list[next].x / 100, list[next].z / 100}, 0}, 0, 1));
	}*/
	t_node *n = create_node(polygons);
	build_node(n);
	post_process_bsp(n, 30, 0);
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
	obj_test(&doom);
	if (!(doom.rendered_area = (float *)malloc(doom.screen.width * doom.screen.height * sizeof(float))))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	free_node(doom.bsp);
	return (0);
}
