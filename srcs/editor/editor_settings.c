/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:01:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/17 15:00:48 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_bool	is_settings_open(t_editor *editor)
{
	return (editor->settings.open);
}

t_bool	editor_settings_guis(t_editor *editor)
{
	editor->settings.guis[ES_GUI_ROOM] = (t_gui){ .render = g_es_room_render,
		.on_enter = g_es_room_enter };
	editor->settings.guis[ES_GUI_OBJECT] = (t_gui){
		.render = g_es_object_render, .on_enter = g_es_object_enter,
		.on_leave = g_es_object_leave, .on_event = g_es_object_on_event };
	editor->settings.guis[ES_GUI_WALL] = (t_gui){ .render = g_es_wall_render,
		.on_enter = g_es_wall_enter };
	editor->settings.guis[ES_GUI_GLOBAL] = (t_gui){
		.render = g_es_global_render, .on_enter = g_es_global_enter };
	editor->settings.guis[ES_GUI_PLAYER] = (t_gui){
		.render = g_es_player_render, .on_enter = g_es_player_enter };
	return (TRUE);
}

void	set_gui_settings(t_editor *editor, int id)
{
	if (editor->settings.current_gui >= 0)
		leave_gui(editor->doom, editor->settings.guis,
			editor->settings.current_gui);
	editor->settings.current_gui = id;
	if (editor->settings.current_gui >= 0)
		enter_gui(editor->doom, editor->settings.guis,
			editor->settings.current_gui);
}

void	editor_settings_update(t_editor *editor)
{
	editor->settings.open = TRUE;
	if (editor->selected_tool == TOOL_PLAYER)
		set_gui_settings(editor, ES_GUI_PLAYER);
	else if (editor->current_room != -1 && editor->current_seg.x == -1
		&& editor->rooms->values[editor->current_room].closed)
		set_gui_settings(editor, ES_GUI_ROOM);
	else if (editor->current_room != -1 && editor->current_seg.x != -1
		&& editor->rooms->values[editor->current_room].closed)
		set_gui_settings(editor, ES_GUI_WALL);
	else if (editor->current_object != -1)
		set_gui_settings(editor, ES_GUI_OBJECT);
	else
		set_gui_settings(editor, ES_GUI_GLOBAL);
}
