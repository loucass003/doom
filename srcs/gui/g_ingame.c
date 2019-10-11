/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/11 00:59:48 by llelievr         ###   ########.fr       */
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
	if (!alloc_components(self, 1))
		return ;
	self->components[0] = create_progress((SDL_Rect){ 5, 5, 200, 30 });
	((t_progress *)self->components[0])->value = 50;
	((t_progress *)self->components[0])->bg_color = 0xFFFF0000;
	((t_progress *)self->components[0])->fg_color = 0xFF00FF00;
}

int i = 0;

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	doom->main_context.image = &doom->screen;
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = 0;
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
		if (r->has_hitbox)
			update_hitbox(r);
		
		render_renderable(&doom->main_context, r);
		if (r->has_hitbox && r->show_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_renderable *sphere = &doom->sphere_primitive;
			t_collide_ellipsoid	*ellipsoid = &r->hitbox.data.ellipsoid;
			sphere->position = ellipsoid->origin;
			sphere->scale = ellipsoid->radius;
			sphere->wireframe = TRUE;
			sphere->wireframe_color = 0xFFFF0000;
			sphere->dirty = TRUE;
			sphere->double_faced = FALSE;
			render_renderable(&doom->main_context, sphere);
		}
	}

	doom->main_context.image->pixels[(doom->main_context.image->height / 2) * doom->main_context.image->width + doom->main_context.image->width / 2 ] = 0xFF00FF00;
	doom->main_context.image = &doom->screen_transparency;


	((t_progress *)self->components[0])->value = doom->player.entity.life * (1 / doom->player.entity.max_life) * 100;
	render_components(doom, self);
}
