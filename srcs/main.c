/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 03:14:10 by llelievr         ###   ########.fr       */
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

void	free_map(t_doom *doom)
{
	free_renderables(&doom->renderables, TRUE);
	free_rooms(&doom->editor.rooms);
	free_objects(&doom->editor.objects, TRUE);
	ft_memdel((void **)&doom->editor.points);
	ft_memdel((void **)&doom->lights);
}

int		exit_doom(t_doom *doom)
{
	threads_destroy(&doom->threads);
	ft_memdel((void **)&doom->main_context.buffer);
	save_datapack(doom);
	free_renderable(&doom->sphere_primitive, FALSE, TRUE, TRUE);
	free_map(doom);
	free_ressources_manager(&doom->res_manager);
	sdl_quit(doom);
	quit_openal(doom);
	if (doom->obj_working_dir)
		ft_memdel((void **)&doom->obj_working_dir);
	return (0);
}

t_doom	init_doom(void)
{
	t_doom doom;

	doom = (t_doom) {
		.running = TRUE,
		.main_context = {.type = CTX_NORMAL, .camera = NULL},
		.current_gui = -1,
		.skybox_index = -1,
		.skybox_enabled = TRUE,
		.editor = {
			.map_renderable = -1, .player_set = FALSE, .current_object = -1 }};
	return (doom);
}

int		main(int argc, char **argv)
{
	t_doom doom;

	doom = init_doom();
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
		return (exit_doom(&doom));
	}
	doom.main_context.doom = &doom;
	doom.editor.doom = &doom;
	if (!init_threads(&doom.threads))
		return (exit_doom(&doom));
	if (!(doom.main_context.buffer = (float *)malloc((int)(S_WIDTH * S_HEIGHT)
		* sizeof(float))))
		return (exit_doom(&doom));
	if (!create_ellipsoid(&doom, &doom.sphere_primitive, (t_vec2){ 12, 12 },
		(t_vec3){ 1, 1, 1 }))
		return (exit_doom(&doom));
	init_sdl(&doom);
	game_loop(&doom);
	exit_doom(&doom);
	return (0);
}
