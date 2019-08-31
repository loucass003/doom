/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/31 18:03:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arrays.h"
#include "doom.h"

t_line	lines[] = {
	{ .a = { 0, 0 },  .b = { 500, 0 } },
	{ .a = { 500, 0 },  .b = { 500, 200 } },
	{ .a = { 500, 200 },  .b = { 250, 200 } },
	{ .a = { 250, 200 },  .b = { 250, 500 } },
	{ .a = { 250, 500 },  .b = { 500, 500 } },
	{ .a = { 500, 500 }, .b = { 500, 700 } },
	{ .a = { 500, 700 }, .b = { 250, 700 }},
	{ .a = { 250, 700},  .b = { 250, 900 }},
	{ .a = { 250, 900},  .b = { 500, 900 }},
	{ .a = { 500, 900},  .b = { 500, 1100 }},
	{ .a = { 500, 1100},  .b = { 0, 1100 }},
	{ .a = { 0, 1100},  .b = { 0, 0 }}
};
// t_line	lines2[] = {
// 	{ .a = { 0, 150 },  .b = { 500, 75 } },
// 	{ .a = { 500, 75 }, .b = { 500, 300 } },
// 	{ .a = { 500, 300 }, .b = { 800, 300 }},
// 	{ .a = { 800, 300},  .b = { 800, 450 }},
// 	{ .a = { 800, 450},  .b = { 0, 450 }},
// 	{ .a = { 0, 450},  .b = { 0, 150 }},
// };


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
	// int count2 = sizeof(lines2) / sizeof(t_line);
	t_polygons *polygons = create_polygons_array(count + 2);
	
	t_3dvertices *vertices = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices, (t_vec3){
			lines[count - 1 - i].a.x / 100, 0, lines[count - 1 - i].a.y / 100});
	}
	append_polygons_array(&polygons, create_polygon(vertices, P_FLOOR, doom->textures.bricks));
	
	t_3dvertices *vertices_ceil = create_3dvertices_array(count);
	for (int i = 0; i < count; i++)
	{
		append_3dvertices_array(&vertices_ceil, (t_vec3){
			lines[i].a.x / 100, 1.5, lines[i].a.y / 100});
	}
	append_polygons_array(&polygons, create_polygon(vertices_ceil, P_CEILING, doom->textures.bricks));
	
	for (int i = 0; i < count; i++)
	{
		append_polygons_array(&polygons, create_wall_polygon(doom->textures.bricks, (t_line){ (t_vec2){lines[i].a.x / 100, (lines[i].a.y / 100)}, (t_vec2){lines[i].b.x / 100, (lines[i].b.y / 100)}}, 0, 1.5));
	}

	// t_polygons *polygons = create_polygons_array(10);
	
	// append_polygons_array(&polygons, create_wall_polygon(doom->textures.bricks, (t_line){  (t_vec2){0, 20}, (t_vec2){0, 0}}, 0, 20));
	// append_polygons_array(&polygons, create_wall_polygon(doom->textures.bricks, (t_line){  (t_vec2){5, 15}, (t_vec2){5, 5}}, 5, 15));

	doom->polygons = polygons;
	post_process_polygons(doom);
	init_lightning(doom);
}

int		main(void)
{
 	t_doom doom = (t_doom) {
		.running = TRUE,
		.main_context = {
			.type = CTX_NORMAL,
			.camera = NULL
		},
		.current_gui = -1
	};

	//doom.main_context.doom = &doom;
	if (!(doom.main_context.buffer = (t_zbuff *)malloc((int)(S_WIDTH * S_HEIGHT) * sizeof(t_zbuff))))
		return (-1);
	init_sdl(&doom);
	init_bsp(&doom);
	obj_test(&doom);
	
	game_loop(&doom);
	sdl_quit(&doom);
	quit_openal();
	//free_node(doom.bsp);
	return (0);
}
