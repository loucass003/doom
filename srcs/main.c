/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/25 06:51:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <errno.h>
#include "arrays.h"
#include "doom.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"

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
	// t_renderable floor;
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
	// }

	// t_renderable sprite;
	// create_sprite(&sprite, (t_mtl){ .texture_map = doom->textures.sprite, .texture_map_set = TRUE }, (t_vec2){ 8, 7 });
	// set_current_cell(&sprite, 0, 0);
	// sprite.scale = (t_vec3){ 5, 6, 5 };
	// //sprite.of.data.sprite->always_facing_player = FALSE;
	// sprite.entity = ft_memalloc(sizeof(t_entity));
	// //-152.831345 2.512500 -25.893129
	// sprite.entity->position = (t_vec3){ -152.831345, 2.512500, -25.893129 };
	// //sprite.entity->position = (t_vec3){ -25.893129, 2.512500, 152.831345 };
	// sprite.entity->type = ENTITY_ENEMY;
	// sprite.entity->packet.doom = doom;
	// sprite.entity->radius = (t_vec3){ 1, 2.5f, 1 };
	// sprite.entity->pos_offset.y = -2.5;
	// // sprite.entity->velocity.x += 4;
	// append_renderables_array(&doom->renderables, sprite);
	t_renderable sprite;
	create_sprite(&sprite, (t_mtl){ .texture_map = doom->textures.machin, .texture_map_set = TRUE }, (t_vec2){ 1, 1 });
	// set_current_cell(&sprite, 0, 0);
	sprite.scale = (t_vec3){ 5, 6, 5 };
	sprite.position.y = sprite.scale.y / 2;
	sprite.show_hitbox = TRUE;
	sprite.hitbox_offset.y -= 1.5;
	t_sprite *s = sprite.sprite;
	s->hitbox_radius = (t_vec3){ 1.5, 2, 1.5 };
	compute_sprite_hitbox(&sprite);
	// sprite.child = malloc(sizeof(t_renderable));
	// create_ellipsoid(sprite.child, 0xFFFF0000, (t_vec2){ 30, 30 }, s->radius);
	// sprite.child->position = sprite.position;
	// sprite.child->position.y += s->radius.y / 2;
	// sprite.child->wireframe = TRUE;
	// sprite.child->wireframe_color = 0xFFFF0000;
	append_renderables_array(&doom->renderables, sprite);

	t_renderable ellipsoid;
	create_ellipsoid(&ellipsoid, 0xFFFF0000, (t_vec2){ 30, 30 }, (t_vec3){ 1, 1, 1 });
	ellipsoid.position = (t_vec3){ 5, 0.5, 5 };
	ellipsoid.scale = (t_vec3){ 1, 1, 1 };
	ellipsoid.wireframe = TRUE;
	ellipsoid.wireframe_color = 0xFFFF0000;
	append_renderables_array(&doom->renderables, ellipsoid);

	t_renderable itemstack;
	t_item *weapon = create_item_weapon_gun(doom->textures.gun0, doom->textures.gun0);
	t_item *ammo = create_item_ammo(doom->textures.ammo1);
	t_item *heal = create_item_heal(doom->textures.medkit);
	create_itemstack_renderable(&itemstack, weapon, 1);
	itemstack.position = (t_vec3){ -7, 1.5, 58 };
	append_renderables_array(&doom->renderables, itemstack);
	create_itemstack_renderable(&itemstack, ammo, 50);
	itemstack.position = (t_vec3){ -10, 1.5, 58 };
	append_renderables_array(&doom->renderables, itemstack);
	create_itemstack_renderable(&itemstack, ammo, 50);
	itemstack.position = (t_vec3){ -13, 1.5, 58 };
	append_renderables_array(&doom->renderables, itemstack);
	create_itemstack_renderable(&itemstack, heal, 1);
	itemstack.position = (t_vec3){ -16, 1.5, 58 };
	append_renderables_array(&doom->renderables, itemstack);

	post_process_map(doom);
}

int		main(int argc, char **argv)
{
 	t_doom doom = (t_doom) {
		.running = TRUE,
		.main_context = {
			.type = CTX_NORMAL,
			.camera = NULL
		},
		.current_gui = -1,
		.skybox_index = -1
	};

	if (argc == 2)
		load_datapack(&doom, argv[1]);

	doom.main_context.doom = &doom;
	//doom.thpool = at_thpool_create (20);
	//pthread_mutex_init(&doom.mutex, NULL);


	if (!(doom.main_context.buffer = (float *)malloc((int)(S_WIDTH * S_HEIGHT) * sizeof(float))))
		return (-1);
	if (!(doom.renderables = create_renderables_array(50)))
		return (-1);
	if (!create_ellipsoid(&doom.sphere_primitive, 0xFFFF0000, (t_vec2){ 12, 12 }, (t_vec3){ 1, 1, 1 }))
		return (-1);
	post_process_obj(&doom, &doom.sphere_primitive);
	//doom.sphere_primitive.octree = create_octree(&doom, &doom.sphere_primitive);
	init_sdl(&doom);
	obj_test(&doom);
	init_bsp(&doom);
	init_lightning(&doom);
	game_loop(&doom);
	save_datapack(&doom);
	sdl_quit(&doom);
	quit_openal();
	return (0);
}
