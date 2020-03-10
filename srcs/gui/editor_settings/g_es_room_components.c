/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_room_components.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:34:23 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 19:35:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

void			g_es_room_cmps_floor(t_gui *self, t_doom *doom, int x, int y)
{
	t_room *room;

	room = &doom->editor.rooms->values[doom->editor.current_room];
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 40, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 40, 40, 40 }, NULL, ">"));
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 10, y + 100 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[2])->value =
		room->floor_invisible;
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 170, y + 100 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[3])->value =
		room->floor_collision;
	append_components_array(&self->components,
		create_select((SDL_Rect){x + 10, y + 120, 300, 30}, "NORMAL TYPE"));
	((t_select *)self->components->values[4])->items = normals_types();
	((t_select *)self->components->values[4])->selected_item =
		select_items_indexof(((t_select *)self->components->values[4])->items,
			room->floor_normal);
}

void			g_es_room_cmps_ceilling(t_gui *self, t_doom *doom, int x, int y)
{
	t_room *room;

	room = &doom->editor.rooms->values[doom->editor.current_room];
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 185, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 185, 40, 40 }, NULL, ">"));
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 10, y + 245 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[7])->value =
		room->ceil_invisible;
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 170, y + 245 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[8])->value =
		room->ceil_collision;
	append_components_array(&self->components,
		create_select((SDL_Rect){x + 10, y + 265, 300, 30}, "NORMAL TYPE"));
	((t_select *)self->components->values[9])->items = normals_types();
	((t_select *)self->components->values[9])->selected_item =
		select_items_indexof(((t_select *)self->components->values[9])->items,
			room->ceil_normal);
}

void			g_es_room_action_floor(t_component *cmp, t_doom *doom,
	t_room *room)
{
	const t_editor	*editor = &doom->editor;
	int				i;

	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[0])
		room->floor_texture = get_prev_ressource(&doom->res_manager,
			room->floor_texture, RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[1])
		room->floor_texture = get_next_ressource(&doom->res_manager,
			room->floor_texture, RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[2])
		room->floor_invisible = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[3])
		room->floor_collision = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[4])
		room->floor_normal = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item].value;
	i = -1;
	while (++i < 5)
		if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[i])
			update_floor((t_editor*)editor, editor->current_room, TRUE);
}

void			g_es_room_action_ceil(t_component *cmp, t_doom *doom,
	t_room *room)
{
	const t_editor	*editor = &doom->editor;
	int				i;

	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[5])
		update_floor((t_editor*)editor, editor->current_room, FALSE);
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[6])
		update_floor((t_editor*)editor, editor->current_room, FALSE);
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[7])
		room->ceil_invisible = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[8])
		room->ceil_collision = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[9])
		room->ceil_normal = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item].value;
	i = -1;
	while (++i < 5)
		if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[5 + i])
			update_floor((t_editor*)editor, editor->current_room, FALSE);
}

t_bool			g_es_room_action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*editor = &doom->editor;
	t_room			*room;

	room = &doom->editor.rooms->values[doom->editor.current_room];
	g_es_room_action_floor(cmp, doom, room);
	g_es_room_action_ceil(cmp, doom, room);
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[10])
	{
		room->ambiant_light = AMBIANT_LIGHT;
		if (((t_textfield *)cmp)->value > 255
			|| ((t_textfield *)cmp)->value < 0)
			((t_textfield *)cmp)->error = TRUE;
		if (!((t_textfield *)cmp)->error)
			room->ambiant_light = ((t_textfield *)cmp)->value;
	}
	return (TRUE);
}
