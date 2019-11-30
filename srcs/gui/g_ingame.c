/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/30 22:50:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"
#include <limits.h>
#include "gui.h"
#include "octree.h"

void static	action_performed(t_component *cmp, t_doom *doom)
{
	
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	doom->screen.secure = FALSE;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	append_components_array(&self->components, create_progress((SDL_Rect){ 5, 5, 200, 30 }));
	((t_progress *)self->components->values[0])->value = 50;
	((t_progress *)self->components->values[0])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[0])->fg_color = 0xFF00FF00;
	append_components_array(&self->components, create_progress((SDL_Rect){ S_WIDTH_2 - 200, 5, 400, 20 }));
	((t_progress *)self->components->values[1])->value = 50;
	((t_progress *)self->components->values[1])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[1])->fg_color = 0xFF00FF00;
}

void	g_ingame_on_leave(t_gui *self, t_doom *doom)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}



void	g_ingame_render(t_gui *self, t_doom *doom)
{
	static float ticks = 0;

	doom->main_context.image = &doom->screen;
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = 0;
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].position = doom->main_context.camera->pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}
	//printf("START FAME ------------\n");
	if (doom->main_context.type == CTX_EDITOR)
	{
		t_renderable *sphere = &doom->sphere_primitive;
		sphere->position = doom->editor.player_pos;
		sphere->scale = doom->player.entity.radius;
		sphere->wireframe = TRUE;
		sphere->wireframe_color = 0xFF5C5C5C;
		sphere->dirty = TRUE;
	//	sphere->double_faced = FALSE;
		render_renderable(&doom->main_context, sphere);
	}
	doom->closer_boss = NULL;
	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable	*r = doom->renderables->values + i;
		if (r->of.type == RENDERABLE_ENTITY)
		{
			entity_update(doom, r->of.data.entity, doom->stats.delta);
			if (r->of.data.entity->type == ENTITY_BOSS && ft_vec3_len(ft_vec3_sub(doom->player.entity.position, r->of.data.entity->position)) <= 50)
				doom->closer_boss = r->of.data.entity;
		}
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
			render_renderable(&doom->main_context, sphere);
		}
	}

	for (int i = 0; i < doom->lights->len; i++)
	{
		t_renderable	*sphere = &doom->sphere_primitive;
		t_light			*light = &doom->lights->values[i];
		sphere->position = light->position;
		sphere->scale = (t_vec3){ 0.2, 0.2, 0.2 };
		sphere->wireframe = TRUE;
		sphere->wireframe_color = 0xFFFF0000;
		sphere->dirty = TRUE;
		//sphere->double_faced = FALSE;
		render_renderable(&doom->main_context, sphere);
	}

	doom->main_context.image->pixels[(doom->main_context.image->height / 2) * doom->main_context.image->width + doom->main_context.image->width / 2 ] = 0xFF00FF00;
	draw_circle(doom->main_context.image, (t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FF00 }, 10);
	doom->main_context.image = &doom->screen_transparency;


	doom->main_context.image = &doom->screen;
	if (doom->main_context.type == CTX_NORMAL)
	{
		int i = -1;
		while (++i < PLAYER_INV_SIZE)
		{
			t_itemstack		*is = &doom->player.item[i];
			if (i == doom->player.selected_slot)
				fill_rect(doom->main_context.image, (SDL_Rect){ 8, 48 + i * 60, 54, 54 }, 0xFFFFFF00);
			fill_rect(doom->main_context.image, (SDL_Rect){ 10, 50 + i * 60, 50, 50 }, 0xFFFF0000);
			if (is->of)
			{
				apply_image_blended(doom->main_context.image, is->of->image->data.texture, is->of->bounds, (SDL_Rect){ 10, 50 + i * 60, 50, 50 });
				if (is->amount <= 1)
					continue;
				const SDL_Color	color = {255, 255, 255, 0};
				SDL_Surface		*text;

				text = TTF_RenderText_Blended(doom->fonts.helvetica,
					ft_int_to_str(is->amount).str, color);
				apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
					(SDL_Rect){ 40, 50 + i * 60, 20, 20 });
				SDL_FreeSurface(text);
			}
		}

		ticks += doom->stats.delta * 30.;

		t_itemstack	*is = &doom->player.item[doom->player.selected_slot]; 

		if (is->of && is->of->type == ITEM_WEAPON)
		{
			t_weapon	*weapon = &is->of->data.weapon;
			if (ticks > 1 && weapon->fireing)
			{
				ticks = 0;
				weapon->current_step++;
				if (weapon->current_step == weapon->steps_count)
				{
					weapon->current_step = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) ? 0 : weapon->idle_step);
					weapon->fireing = FALSE;
				}
				set_current_animation_step(weapon, weapon->animation_seq[weapon->current_step]);
			}
			apply_image_blended(doom->main_context.image, weapon->animation->data.texture, weapon->curr_image, (SDL_Rect){ S_WIDTH_2 - 80 / 2, S_HEIGHT - 300, 300, 300 });
		}

		((t_progress *)self->components->values[0])->value = doom->player.entity.life * (1 / doom->player.entity.max_life) * 100;
		if (doom->player.entity.life <= 0)
		{
			set_gui(doom, GUI_GAMEOVER);
			return ;
		}
	}

	self->components->values[0]->visible = doom->main_context.type == CTX_NORMAL;
	self->components->values[1]->visible = doom->main_context.type == CTX_NORMAL && !!doom->closer_boss;
	if (doom->closer_boss)
	{
		((t_progress *)self->components->values[1])->value = doom->closer_boss->life * (1 / doom->closer_boss->max_life) * 100;
	}

	render_components(doom, self);
	doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS], doom);
}
