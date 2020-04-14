/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 01:37:16 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 01:38:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

t_bool		gui_ingame_editor_events(t_doom *doom, t_editor *editor,
	t_gui *self, SDL_Event *event)
{
	if (!doom->mouse_focus && is_settings_open(&doom->editor))
	{
		g_editor_settings_on_event(self, event, doom);
		return (FALSE);
	}
	if (event->type == SDL_MOUSEBUTTONDOWN
		&& event->button.button == SDL_BUTTON_LEFT)
		event_button_left(doom);
	else if (event->type == SDL_MOUSEBUTTONDOWN
		&& event->button.button == SDL_BUTTON_RIGHT)
		unselect_all(doom);
	if (event->type == SDL_KEYDOWN && doom->editor.current_room != -1
		&& (doom->editor.wall_section != -1
			|| doom->editor.selected_floor_ceil != -1))
		uvs_events(doom, event);
	if (event->type == SDL_KEYDOWN && doom->editor.selected_floor_ceil != -1
		&& doom->editor.current_room != -1)
		ceil_floor_events(doom, event);
	if (event->type == SDL_KEYDOWN && doom->editor.current_object != -1)
		object_events(doom, event);
	return (TRUE);
}
