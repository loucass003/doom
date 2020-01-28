/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/28 14:27:30 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arrays.h"
#include "doom.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"

void	init_bsp(t_doom *doom)
{
	t_renderable r;

	create_player(&r, doom);
	append_renderables_array(&doom->renderables, r);

	t_renderable ellipsoid;
	create_ellipsoid(doom, &ellipsoid, (t_vec2){ 30, 30 }, (t_vec3){ 1, 1, 1 });
	ellipsoid.materials->values[0].material_color = 0xFFFF0000;
	ellipsoid.position = (t_vec3){ 5, 0.5, 5 };
	ellipsoid.scale = (t_vec3){ 1, 1, 1 };
	ellipsoid.wireframe_color = 0xFFFF0000;
	append_renderables_array(&doom->renderables, ellipsoid);
	



	t_renderable itemstack;
	t_item *weapon = create_item_weapon_gun(doom->res_manager.ressources->values[2], doom->res_manager.ressources->values[3]);
	t_item *axe = create_item_weapon_axe(doom->res_manager.ressources->values[2], doom->res_manager.ressources->values[4]);
	t_item *ammo = create_item_ammo(doom->res_manager.ressources->values[2]);
	t_item *heal = create_item_heal(doom->res_manager.ressources->values[2]);
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
	create_itemstack_renderable(&itemstack, axe, 1);
	itemstack.position = (t_vec3){ -19, 1.5, 58 };
	append_renderables_array(&doom->renderables, itemstack);


	set_obj_working_dir(doom, "assets/obj/cs_italy");
	create_obj(doom, &r, "cs_italy.obj");
	r.position = (t_vec3){0, 0, -3};
	r.rotation = (t_vec3){0, 0, 0};
	r.scale = (t_vec3){0.06, 0.06, 0.06};
	//r.scale = (t_vec3){5, 5, 5};
	//r.wireframe = TRUE;
	r.wireframe_color = 0xFFFF0000;
	r.fixed = TRUE;
	append_renderables_array(&doom->renderables, r);

	set_obj_working_dir(doom, "assets/obj");
	create_obj(doom, &r, "House2.obj");
	r.position = (t_vec3){-10, 0, 0};
	r.scale = (t_vec3){0.03, 0.03, 0.03};
	r.fixed = TRUE;
	append_renderables_array(&doom->renderables, r);


	

//	post_process_map(doom);
}

t_bool	init_map(t_doom *doom)
{
	if (!(doom->editor.rooms = create_rooms_array(15)))
		return (FALSE);
	if (!(doom->editor.points = create_2dvertices_array(50)))
		return (FALSE);
	if (!(doom->editor.objects = create_objects_array(15)))
		return (FALSE);
	if (!(doom->renderables = create_renderables_array(50)))
		return (FALSE);
	init_lightning(doom);
	return (TRUE);
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
		.skybox_index = -1,
		.skybox_enabled = TRUE,
		.editor = { .map_renderable = -1, .player_set = FALSE }
	};

	if (argc != 2)
	{
		ft_putendl("Usage: ./doom-nukem <datapack>");
		return (0);
	}
	init_openal(&doom);
	init_ressources_registry(&doom);
	init_map(&doom);

	if (!load_datapack(&doom, argv[1]))
	{
		ft_putendl("Error: invalid datapack");
		return (0);
	}

	doom.main_context.doom = &doom;
	doom.editor.doom = &doom;

	if (!init_threads(&doom.threads))
		return (-1);
	if (!(doom.main_context.buffer = (float *)malloc((int)(S_WIDTH * S_HEIGHT) * sizeof(float))))
		return (-1);
	if (!create_ellipsoid(&doom, &doom.sphere_primitive, (t_vec2){ 12, 12 }, (t_vec3){ 1, 1, 1 }))
		return (-1);
	
	init_sdl(&doom);

	game_loop(&doom);
	save_datapack(&doom);
	sdl_quit(&doom);
	alDeleteSources(MAX_SOUNDS, (const ALuint *)&doom.audio.source);
	quit_openal();
	return (0);
}
