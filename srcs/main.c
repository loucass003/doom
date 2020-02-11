/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 03:03:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arrays.h"
#include "doom.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"

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
		.editor = { .map_renderable = -1, .player_set = FALSE, .current_object = -1 }
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
