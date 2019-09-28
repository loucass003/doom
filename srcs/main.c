/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/28 19:11:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arrays.h"
#include "doom.h"
#include "octree.h"

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
	//  t_renderable floor;
	// create_polygon(&floor, P_FLOOR, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE });
	// for (int i = 0; i < count; i++)
	// {
	// 	add_polygon_vertice(&floor, (t_vec4){lines[i].a.x , 0, lines[i].a.y , 1});
	// }
	// append_renderables_array(&doom->renderables, floor);
	
	// t_renderable ceiling;
	// create_polygon(&ceiling, P_CEILING, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE });
	// for (int i = 0; i < count; i++)
	// {
	// 	add_polygon_vertice(&ceiling, (t_vec4){ lines[count - 1 - i].a.x , 150, lines[count - 1 - i].a.y, 1});
	// }
	
	// append_renderables_array(&doom->renderables, ceiling);
	
	// for (int i = 0; i < count; i++)
	// {
	// 	t_renderable wall;
	// 	create_wall_polygon(&wall, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE }, (t_line){ (t_vec2){lines[i].a.x , (lines[i].a.y )}, (t_vec2){lines[i].b.x , (lines[i].b.y )}}, (t_vec2){0, 150});
	// 	wall.scale = (t_vec3){ 0.1, 0.1, 0.1 };
	// 	append_renderables_array(&doom->renderables, wall);
	//}

	// t_renderable tr;
	// create_wall_polygon(&tr, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE }, (t_line){(t_vec2){2.5, 8}, (t_vec2){0, 8}}, (t_vec2){0, 1.5});
	// append_renderables_array(&doom->renderables, tr);

	// t_ptrs	*ptrs = create_ptrs_array(1);

	// append_ptrs_array(&ptrs, polygons);
	// append_ptrs_array(&ptrs, &polygons->polygons[0]);

	// t_polygons *arr0 = (t_polygons *)ptrs->values[0];
	// t_polygon *poly0 = (t_polygon *)ptrs->values[1];

	t_renderable sprite;
	create_sprite(&sprite, (t_mtl){ .texture_map = doom->textures.sprite, .texture_map_set = TRUE });
	sprite.scale = (t_vec3){ 5, 6, 5 };
	sprite.position = (t_vec3){ 0, 0, 0 };
	append_renderables_array(&doom->renderables, sprite);

	post_process_map(doom);
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

	doom.main_context.doom = &doom;
	if (!(doom.main_context.buffer = (float *)malloc((int)(S_WIDTH * S_HEIGHT) * sizeof(float))))
		return (-1);
	if (!(doom.renderables = create_renderables_array(50)))
		return (-1);
	init_sdl(&doom);
	obj_test(&doom);
	init_bsp(&doom);
	init_lightning(&doom);
	//init_openal(&doom);
	game_loop(&doom);
	sdl_quit(&doom);
	quit_openal();
	//free_node(doom.bsp);
	return (0);
}
