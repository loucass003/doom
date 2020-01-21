/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/20 18:41:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	t_room *room = &doom->editor.rooms->values[doom->editor.current_room];
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[0])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->floor_texture = get_prev_ressource(&doom->res_manager, room->floor_texture, RESSOURCE_TEXTURE);
		update_floor(editor, editor->current_room, TRUE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[1])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->floor_texture = get_next_ressource(&doom->res_manager, room->floor_texture, RESSOURCE_TEXTURE);
		update_floor(editor, editor->current_room, TRUE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[2])
	{
		room->floor_visible = ((t_checkbox *)cmp)->value;
		update_floor(editor, editor->current_room, TRUE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[3])
	{
		room->floor_collision = ((t_checkbox *)cmp)->value;
		update_floor(editor, editor->current_room, TRUE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[4])
	{
		room->floor_normal = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;
		update_floor(editor, editor->current_room, TRUE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[5])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->ceiling_texture = get_prev_ressource(&doom->res_manager, room->ceiling_texture, RESSOURCE_TEXTURE);
		update_floor(editor, editor->current_room, FALSE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[6])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->ceiling_texture = get_next_ressource(&doom->res_manager, room->ceiling_texture, RESSOURCE_TEXTURE);
		update_floor(editor, editor->current_room, FALSE);
	}
	
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[7])
	{
		room->ceil_visible = ((t_checkbox *)cmp)->value;
		update_floor(editor, editor->current_room, FALSE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[8])
	{
		room->ceil_collision = ((t_checkbox *)cmp)->value;
		update_floor(editor, editor->current_room, FALSE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[9])
	{
		room->ceil_normal = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;
		update_floor(editor, editor->current_room, FALSE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[10])
	{
		room->ambiant_light = AMBIANT_LIGHT;
		if (((t_textfield *)cmp)->value > 255 || ((t_textfield *)cmp)->value < 0)
			((t_textfield *)cmp)->error = TRUE;
		if (!((t_textfield *)cmp)->error)
			room->ambiant_light = ((t_textfield *)cmp)->value;
	}
 	return (TRUE);
}

void			g_es_room_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	t_room *room = &doom->editor.rooms->values[doom->editor.current_room];
	
	t_select_items	*normals_types = create_select_items_array(3);
	append_select_items_array(&normals_types, (t_select_item){ .name = "FRONT", .value = 0 });
	append_select_items_array(&normals_types, (t_select_item){ .name = "BACK", .value = 1 });
	append_select_items_array(&normals_types, (t_select_item){ .name = "DOUBLE SIDED", .value = 2 });

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 40, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 40, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 100 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[2])->value = room->floor_visible; //floor_visible
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 170, y + 100 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[3])->value = room->floor_collision;
	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 120, 300, 30}, "NORMAL TYPE"));
	((t_select *)self->components->values[4])->items = normals_types;
	((t_select *)self->components->values[4])->selected_item = select_items_indexof(normals_types, room->floor_normal);

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 185, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 185, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 245 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[7])->value = room->ceil_visible; //ceiling_visible
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 170, y + 245 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[8])->value = room->ceil_collision;
	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 265, 300, 30}, "NORMAL TYPE"));
	((t_select *)self->components->values[9])->items = normals_types;
	((t_select *)self->components->values[9])->selected_item = select_items_indexof(normals_types, room->ceil_normal);
	
	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 325, 300, 30}, "AMBIANT LIGHT", TRUE));
	t_int_str istr = ft_int_to_str(room->ambiant_light);
	set_text_value((t_textfield *)self->components->values[10], istr.str, istr.len);

	int i = -1;
	while (++i < 11)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_room_render(t_gui *self, t_doom *doom)
{
	t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];

	SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE FLOOR", (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 80, 220, 20});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE CEILING", (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 230, 220, 20});
	SDL_FreeSurface(text);
	if (curr_room->floor_texture && curr_room->floor_texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img_floor = curr_room->floor_texture->data.texture;
		apply_image_to_image(&doom->screen, img_floor, (SDL_Rect){ 0, 0, img_floor->width, img_floor->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 115, 220, 40 });
	}
	if (curr_room->ceiling_texture && curr_room->ceiling_texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img_ceiling = curr_room->ceiling_texture->data.texture;
		apply_image_to_image(&doom->screen, img_ceiling, (SDL_Rect){ 0, 0, img_ceiling->width, img_ceiling->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 260, 220, 40 });
	}
}
