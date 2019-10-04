/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/04 19:09:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"
#include <limits.h>
#include "gui.h"

void static	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

int i = 0;

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	// if (++i > 1000)
	// {
	// 	sleep(30);
	// }
	doom->main_context.image = &doom->screen;
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = 0;
	/* for (int i = 0; i < doom->polygons->len; i++)
	{
		if (!doom->polygons->polygons[i].transparent)
			render_polygon(&doom->main_context, doom->polygons->polygons + i);
	}
	t_obj *obj = &doom->objects->objs[0];

	render_obj(&doom->main_context, obj);

	doom->main_context.image = &doom->screen_transparency;
	for (int i = 0; i < doom->polygons->len; i++)
	{
		if (doom->polygons->polygons[i].transparent)
			render_polygon(&doom->main_context, doom->polygons->polygons + i);
	} */

	//doom->gdata.all_called = FALSE;
	doom->gdata.todo_triangles = 0;
	doom->gdata.finished_triangles = 0;
	doom->renderables->values[1].rotation.y += 0.01;
	doom->renderables->values[1].dirty = TRUE;
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].position = doom->main_context.camera->pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}
	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable	*r = doom->renderables->values + i;

		if (r->entity)
			entity_update(doom, r->entity, doom->stats.delta);
		render_renderable(&doom->main_context, r);
	}
	doom->main_context.image->pixels[(doom->main_context.image->height / 2) * doom->main_context.image->width + doom->main_context.image->width / 2 ] = 0xFF00FF00;
	// printf("triangles = %d/%d\n", doom->gdata.finished_triangles, doom->gdata.todo_triangles);
	// //doom->gdata.all_called = TRUE;
	// //exit(1);
	// while(doom->gdata.todo_triangles != doom->gdata.finished_triangles)
	// 	; 
//	tpool_wait(doom->thpool);

	// doom->renderables->values[0].rotation.y += 0.03;
	// doom->renderables->values[0].dirty = TRUE;
	doom->main_context.image = &doom->screen_transparency;
}
