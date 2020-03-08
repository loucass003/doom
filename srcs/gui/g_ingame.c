/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/05 16:40:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <limits.h>
#include "gui.h"
#include "octree.h"
#include <math.h>
#include "render.h"
#include "editor.h"
#include "threads.h"
#include "door.h"
#include "script.h"

static t_bool	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[2] && !doom->mouse_focus)
	{
		set_gui(doom, GUI_EDITOR);
		return (FALSE);
	}
	return (TRUE);
}

void			g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	enter_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);

	enter_gui(doom, doom->guis, GUI_MESSAGE); 
	
	doom->screen.secure = FALSE;
	doom->mouse_focus = TRUE;
	append_components_array(&self->components, create_progress((SDL_Rect)
		{ 5, 5, 200, 30 }));
	((t_progress *)self->components->values[0])->value = 50;
	((t_progress *)self->components->values[0])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[0])->fg_color = 0xFF00FF00;
	append_components_array(&self->components, create_progress((SDL_Rect)
		{ S_WIDTH_2 - 200, 5, 400, 20 }));
	((t_progress *)self->components->values[1])->value = 50;
	((t_progress *)self->components->values[1])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[1])->fg_color = 0xFF00FF00;
	if (doom->main_context.type == CTX_EDITOR)
	{
		append_components_array(&self->components, create_button((SDL_Rect)
		{ 10, 10, 200, 50 }, NULL, "BACK"));
		self->components->values[2]->perform_action = action_performed;
	}
}

void			g_ingame_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	leave_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	leave_gui(doom, doom->guis, GUI_MESSAGE);
	doom->mouse_focus = FALSE;
}

void			g_ingame_on_events(t_gui *self, SDL_Event *event, t_doom *doom)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_TAB))
		doom->mouse_focus = !doom->mouse_focus;
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (!doom->mouse_focus && is_settings_open(&doom->editor))
		{
			g_editor_settings_on_event(self, event, doom);
			return ;
		}
		if (event->type == SDL_MOUSEBUTTONDOWN
			&& event->button.button == SDL_BUTTON_LEFT)
			event_button_left(doom);
		else if (event->type == SDL_MOUSEBUTTONDOWN
			&& event->button.button == SDL_BUTTON_RIGHT)
			unselect_all(doom);
		if (event->type == SDL_KEYDOWN && (doom->editor.wall_section != -1 || (doom->editor.selected_floor_ceil != -1 && doom->editor.current_room != -1)))
			uvs_events(doom, event);
		if (event->type == SDL_KEYDOWN && doom->editor.selected_floor_ceil != -1
			&& doom->editor.current_room != -1)
			ceil_floor_events(doom, event);
		if (event->type == SDL_KEYDOWN && doom->editor.current_object != -1)
			object_events(doom, event);
	}
	player_inventory_event(doom, event);
	components_events(doom, doom->guis, event, GUI_EDITOR_SETTINGS);
	g_message_on_event(self, event, doom);
}

void			g_ingame_render(t_gui *self, t_doom *doom)
{
	int i;

	i = -1;
	update_controls(doom);
	doom->main_context.image = &doom->screen;
	while (++i < S_WIDTH * S_HEIGHT)
		doom->main_context.buffer[i] = 0;
	skybox_activated(doom, i);
	threads_clear(&doom->threads);
	if (doom->main_context.type == CTX_EDITOR)
		sphere_context_editor(doom);
	doom->closer_boss = NULL;
	the_ingame_render(doom);
	if (doom->main_context.type == CTX_EDITOR)
		light_render(doom);
	threads_launch(&doom->threads);
	threads_wait(&doom->threads);
	doom->main_context.image->pixels[(doom->main_context.image->height / 2)
		* doom->main_context.image->width
		+ doom->main_context.image->width / 2] = 0xFF00FF00;
	draw_circle(doom->main_context.image,
		(t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FF00 }, 10);
	doom->main_context.image = &doom->screen;
	if (!draw_player_inventory(doom, self))
		return ;
	draw_transforms_type(&doom->editor);
	self->components->values[0]->visible = doom->main_context.type == CTX_NORMAL;
	self->components->values[1]->visible = doom->main_context.type == CTX_NORMAL
		&& !!doom->closer_boss;
	if (doom->closer_boss)
		((t_progress *)self->components->values[1])->value = doom->closer_boss
			->life * (1 / doom->closer_boss->max_life) * 100;
	render_components(doom, self);
	if (doom->main_context.type == CTX_EDITOR)
		doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS], doom);
	if (doom->main_context.type == CTX_NORMAL)
		doom->guis[GUI_MESSAGE].render(&doom->guis[GUI_MESSAGE], doom);
}
