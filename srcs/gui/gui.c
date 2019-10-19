/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 09:46:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/19 17:07:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	register_guis(t_doom *doom)
{
	doom->guis[GUI_INGAME] = (t_gui){ .render = g_ingame_render, .on_enter = g_ingame_on_enter };
	doom->guis[GUI_MAIN_MENU] = (t_gui){ .render = g_mainmenu_render, .on_enter = g_mainmenu_on_enter, .on_leave = g_mainmenu_on_leave };
	doom->guis[GUI_EDITOR] = (t_gui) { .render = g_editor_render, .on_enter = g_editor_on_enter, .on_leave = g_editor_on_leave, .on_event = g_editor_on_event };
	doom->guis[GUI_GAMEOVER] = (t_gui) { .render = g_gameover_render, .on_enter = g_gameover_on_enter, .on_leave = g_gameover_on_leave };
	doom->guis[GUI_RESSOURCES] = (t_gui) { .render = g_ressources_render, .on_enter = g_ressources_on_enter, .on_leave = g_ressources_on_leave };
}

void	set_gui(t_doom *doom, int id)
{
	t_gui	*old_gui;
	t_gui	*new_gui;

	if (doom->current_gui >= 0)
	{
		old_gui = &doom->guis[doom->current_gui];
		if (old_gui->components)
			ft_memdel(&old_gui->components);
		if (old_gui->on_leave != NULL)
			old_gui->on_leave(old_gui, doom);
	}
	doom->current_gui = id;
	new_gui = &doom->guis[doom->current_gui];
	if (new_gui->on_enter != NULL)
	{
		if (!new_gui->components)
			new_gui->components = create_components_array(10);
		new_gui->on_enter(new_gui, doom);
	}
}
