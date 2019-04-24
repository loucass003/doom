/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 09:46:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 16:15:11 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	register_guis(t_doom *doom)
{
	doom->guis[GUI_INGAME] = (t_gui){ .render = g_ingame_render };
	doom->guis[GUI_MAIN_MENU] = (t_gui){ .render = g_mainmenu_render, .on_enter = g_mainmenu_on_enter, .on_leave = g_mainmenu_on_leave };
	doom->guis[GUI_EDITOR] = (t_gui) { .render = g_editor_render, .on_enter = g_editor_on_enter, .on_leave = g_editor_on_leave, .on_event = g_editor_on_event };     
}

void	set_gui(t_doom *doom, int id)
{
	t_gui	*old_gui;
	t_gui	*new_gui;

	old_gui = &doom->guis[doom->current_gui];
	if (old_gui->on_leave != NULL)
		old_gui->on_leave(old_gui);
	doom->current_gui = id;
	new_gui = &doom->guis[doom->current_gui];
	if (new_gui->on_enter != NULL)
		new_gui->on_enter(new_gui);
}
