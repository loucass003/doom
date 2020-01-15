/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 09:46:13 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 16:50:44 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "doom.h"
#include "gui.h"

void	register_guis(t_doom *doom)
{
	doom->guis[GUI_INGAME] = (t_gui){ .render = g_ingame_render, .on_enter = g_ingame_on_enter, .on_leave = g_ingame_on_leave, .on_event = g_ingame_on_events };
	doom->guis[GUI_MAIN_MENU] = (t_gui){ .render = g_mainmenu_render, .on_enter = g_mainmenu_on_enter, .on_leave = g_mainmenu_on_leave };
	doom->guis[GUI_EDITOR] = (t_gui) { .render = g_editor_render, .on_enter = g_editor_on_enter, .on_leave = g_editor_on_leave, .on_event = g_editor_on_event };
	doom->guis[GUI_GAMEOVER] = (t_gui) { .render = g_gameover_render, .on_enter = g_gameover_on_enter, .on_leave = g_gameover_on_leave };
	doom->guis[GUI_RESSOURCES] = (t_gui) { .render = g_ressources_render, .on_enter = g_ressources_on_enter, .on_leave = g_ressources_on_leave, .on_event = g_ressources_on_event };
	doom->guis[GUI_EDITOR_SETTINGS] = (t_gui) { .render = g_editor_settings_render, .on_enter = g_editor_settings_on_enter, .on_leave = g_editor_settings_on_leave, .on_event = g_editor_settings_on_event };
	doom->guis[GUI_LEVEL] = (t_gui) { .render = g_level_render, .on_enter = g_level_on_enter, .on_leave = g_level_on_leave };
	doom->guis[GUI_ESC] = (t_gui) { .render = g_esc_render};
}

void	leave_gui(t_doom *doom, t_gui *guis, int id)
{
	t_gui	*old_gui;

	old_gui = &guis[id];
	if (old_gui->components)
		free_components(old_gui);
	if (old_gui->on_leave != NULL)
		old_gui->on_leave(old_gui, doom);
}

void	enter_gui(t_doom *doom, t_gui *guis, int id)
{
	t_gui	*new_gui;

	new_gui = &guis[id];
	if (new_gui->on_enter != NULL)
	{
		if (!new_gui->components)
			new_gui->components = create_components_array(10);
		new_gui->on_enter(new_gui, doom);
	}
}

void	set_gui(t_doom *doom, int id)
{
	if (doom->current_gui >= 0)
		leave_gui(doom, doom->guis, doom->current_gui);
	doom->current_gui = id;
	if (doom->current_gui >= 0)
		enter_gui(doom, doom->guis, doom->current_gui);
}

void	gui_events(t_doom *doom, t_gui *guis, SDL_Event *event, int id)
{
	t_gui	*gui;

	if (id < 0)
		return ;
	gui = &guis[id];
	if (gui->on_event != NULL)
		gui->on_event(gui, event, doom);
	
}
