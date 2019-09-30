/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/30 16:37:26 by llelievr         ###   ########.fr       */
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
	
}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
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

	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable	*r = doom->renderables->values + i;

		if (r->entity)
			entity_update(r->entity, doom->stats.delta);
		render_renderable(&doom->main_context, r);
	}
	// doom->renderables->values[0].rotation.y += 0.03;
	// doom->renderables->values[0].dirty = TRUE;
	doom->main_context.image = &doom->screen_transparency;
}
