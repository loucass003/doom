/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/26 15:31:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[0])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->floor_texture = get_prev_ressource(&doom->res_manager, room->floor_texture, RESSOURCE_TEXTURE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[1])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->floor_texture = get_next_ressource(&doom->res_manager, room->floor_texture, RESSOURCE_TEXTURE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[2])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->ceiling_texture = get_prev_ressource(&doom->res_manager, room->ceiling_texture, RESSOURCE_TEXTURE);
	}
	if (cmp == editor->settings.guis[ES_GUI_ROOM].components->values[3])
	{
		t_room	*room = &editor->rooms->values[editor->current_room];
		room->ceiling_texture = get_next_ressource(&doom->res_manager, room->ceiling_texture, RESSOURCE_TEXTURE);
	}
	return (TRUE);
}

void			g_es_room_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 40, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 40, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 120, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 120, 40, 40 }, NULL, ">"));
	int i = -1;
	while (++i < 4)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_room_render(t_gui *self, t_doom *doom)
{
	t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];

	SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE FLOOR", (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 85, 220, 20});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE CEILING", (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 20});
	SDL_FreeSurface(text);
	if (curr_room->floor_texture && curr_room->floor_texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img_floor = curr_room->floor_texture->data.texture;
		apply_image_to_image(&doom->screen, img_floor, (SDL_Rect){ 0, 0, img_floor->width, img_floor->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 115, 220, 40 });
	}
	if (curr_room->ceiling_texture && curr_room->ceiling_texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img_ceiling = curr_room->ceiling_texture->data.texture;
		apply_image_to_image(&doom->screen, img_ceiling, (SDL_Rect){ 0, 0, img_ceiling->width, img_ceiling->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 195, 220, 40 });
	}
}
