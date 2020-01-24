/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/24 15:23:48 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

void					g_editor_on_enter(t_gui *self, t_doom *doom)
{
	enter_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	doom->editor.current_point = -1;
	doom->editor.current_object = -1;
	doom->editor.wall_section = -1;
	select_room(&doom->editor, -1);
	doom->editor.settings.open = TRUE;
	g_editor_button(self, doom);
}

void					g_editor_on_event(t_gui *self,
	SDL_Event *event, t_doom *doom)
{
	doom->mouse_focus = FALSE;
	components_events(doom, doom->guis, event, GUI_EDITOR_SETTINGS);
	g_editor_settings_on_event(self, event, doom);
	if (event->type == SDL_MOUSEMOTION)
		editor_mousemotion(event, doom);
	else if (event->type == SDL_MOUSEBUTTONDOWN)
		editor_mousebuttondown(event, doom);
	else if (event->type == SDL_MOUSEBUTTONUP)
		editor_mousebuttonup(doom);
	else if (event->type == SDL_KEYDOWN)
		editor_keydown(event, doom);
}

void					g_editor_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	leave_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
}

void					g_editor_render(t_gui *self, t_doom *doom)
{
	editor_grid_render(doom, &doom->editor);
	editor_render_rooms(doom, &doom->editor);
	editor_render_player(doom, &doom->editor);
	editor_render_objects(&doom->editor);
	draw_rect(&doom->screen, (SDL_Rect){ 5, 5, S_WIDTH - 10, 57 }, 0xFFFFFFFF);
	render_components(doom, self);
	doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS],
		doom);
	((t_button *)doom->guis[doom->current_gui].components->values[5])
		->color_default = !doom->editor.player_set ? 0xFFFF0000 : 0xFF505050;
}
