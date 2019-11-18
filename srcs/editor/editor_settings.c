/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:01:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/18 01:48:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_bool	is_settings_open(t_editor *editor)
{
	return (editor->settings.open && editor->settings.visible);
}

t_bool	editor_settings_guis(t_editor *editor)
{
	int		i;

	editor->settings.guis[ES_GUI_ROOM] = (t_gui){ .render = g_es_room_render, .on_enter = g_es_room_enter };
	editor->settings.guis[ES_GUI_OBJECT] = (t_gui){ .render = g_es_object_render, .on_enter = g_es_object_enter };
	editor->settings.guis[ES_GUI_WALL] = (t_gui){ .render = g_es_wall_render, .on_enter = g_es_wall_enter };
	return (TRUE);
}

void	set_gui_settings(t_editor *editor, int id)
{
	t_gui	*old_gui;
	t_gui	*new_gui;

	if (id == editor->settings.current_gui)
		return ;

	if (editor->settings.current_gui >= 0)
	{
		old_gui = &editor->settings.guis[editor->settings.current_gui];
		if (old_gui->components)
			free_components(old_gui);
		if (old_gui->on_leave != NULL)
			old_gui->on_leave(old_gui, editor->doom);
	}
	editor->settings.current_gui = id;
	if (editor->settings.current_gui >= 0)
	{
		new_gui = &editor->settings.guis[id];
		if (new_gui->on_enter != NULL)
		{
			if (!new_gui->components)
				new_gui->components = create_components_array(10);
			new_gui->on_enter(new_gui, editor->doom);
		}
	}
}

void	editor_settings_update(t_editor *editor)
{
	editor->settings.visible = TRUE;
	if (editor->current_room != -1 && editor->current_seg.x == -1)
		set_gui_settings(editor, ES_GUI_ROOM);
	else if (editor->current_room != -1 && editor->current_seg.x != -1)
		set_gui_settings(editor, ES_GUI_WALL);
	else if (editor->current_object != -1)
		set_gui_settings(editor, ES_GUI_OBJECT);
	else
	{
		editor->settings.current_gui = -1;
		editor->settings.visible = FALSE;
	}
}

