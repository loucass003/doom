/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_wall_actions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 21:20:51 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:22:00 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

void					g_es_wall_action_section(t_component *cmp,
	t_editor *editor, t_wall *wall)
{
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[0])
	{
		editor->wall_section--;
		if (editor->wall_section < 0)
			editor->wall_section = wall->wall_sections->len - 1;
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[1])
	{
		editor->wall_section++;
		if (editor->wall_section >= wall->wall_sections->len)
			editor->wall_section = 0;
	}
}

t_bool					g_es_wall_action_wall_type(t_component *cmp,
	t_editor *editor, int w_index, t_wall_section *ws)
{
	t_renderable *r;

	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[2])
	{
		ws->type = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item].value;
		r = get_map(editor);
		if (r)
		{
			if (ws->type == WS_DOOR)
				hide_adjacent_walls(editor, editor->current_room, w_index, ws);
			add_map(r, editor);
		}
		editor_settings_update(editor);
		return (FALSE);
	}
	return (TRUE);
}

void					g_es_wall_action_settings(t_component *cmp,
	t_doom *doom, t_wall_section *ws)
{
	t_editor		*editor;

	editor = &doom->editor;
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[3])
		ws->texture = get_prev_ressource(&doom->res_manager, ws->texture,
			RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[4])
		ws->texture = get_next_ressource(&doom->res_manager, ws->texture,
			RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[5])
		ws->collisions = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[6])
		ws->invisible = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[7])
		ws->normal_type = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item].value;
}

t_bool					g_es_wall_action_performed(t_component *cmp,
	t_doom *doom)
{
	t_editor		*editor;
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;
	int				wall_index;

	editor = &doom->editor;
	room = &editor->rooms->values[editor->current_room];
	wall_index = wall_indexof_by_indice(room->walls, editor->current_seg.x);
	if (wall_index == -1)
		return (FALSE);
	wall = &room->walls->values[wall_index];
	ws = &wall->wall_sections->values[editor->wall_section];
	g_es_wall_action_section(cmp, editor, wall);
	if (!g_es_wall_action_wall_type(cmp, editor, wall_index, ws))
		return (FALSE);
	g_es_wall_action_settings(cmp, doom, ws);
	update_wall(editor, editor->current_room,
		wall_indexof_by_indice(
			editor->rooms->values[editor->current_room].walls,
			editor->current_seg.x),
		editor->wall_section);
	return (TRUE);
}

void					g_es_wall_components_visibility(t_components *cmps,
	t_wall_section *ws)
{
	int			i;

	i = 2;
	while (++i < 8)
	{
		cmps->values[i]->visible = ws->type == WS_WALL;
		cmps->values[i]->enabled = ws->type == WS_WALL;
	}
}
