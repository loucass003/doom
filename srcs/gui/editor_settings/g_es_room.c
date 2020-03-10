/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 19:39:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_select_items	*normals_types(void)
{
	t_select_items	*normals_types;

	normals_types = create_select_items_array(3);
	append_select_items_array(&normals_types,
		(t_select_item){ .name = "FRONT", .value = 0 });
	append_select_items_array(&normals_types,
		(t_select_item){ .name = "BACK", .value = 1 });
	append_select_items_array(&normals_types,
		(t_select_item){ .name = "DOUBLE SIDED", .value = 2 });
	return (normals_types);
}

void			g_es_room_enter(t_gui *self, t_doom *doom)
{
	const int	x = S_WIDTH - 335;
	const int	y = 75;
	t_room		*room;
	t_int_str	istr;
	int			i;

	room = &doom->editor.rooms->values[doom->editor.current_room];
	g_es_room_cmps_floor(self, doom, x, y);
	g_es_room_cmps_ceilling(self, doom, x, y);
	append_components_array(&self->components,
		create_textfield((SDL_Rect){x + 10, y + 325, 300, 30},
			"AMBIANT LIGHT", TRUE));
	istr = ft_int_to_str(room->ambiant_light);
	set_text_value((t_textfield *)self->components->values[10], istr.str,
		istr.len);
	i = -1;
	while (++i < 11)
		self->components->values[i]->perform_action =
			g_es_room_action_performed;
}

void			g_es_room_render_text(t_doom *doom)
{
	SDL_Surface *t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE FLOOR",
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 80, 220, 20});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE CEILING",
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 230, 220, 20});
	SDL_FreeSurface(t);
}

void			g_es_room_render(t_gui *self, t_doom *doom)
{
	t_room		*room;
	t_img		*img;

	(void)self;
	room = &doom->editor.rooms->values[doom->editor.current_room];
	g_es_room_render_text(doom);
	if (room->floor_texture && room->floor_texture->type == RESSOURCE_TEXTURE)
	{
		img = room->floor_texture->data.texture;
		apply_image_to_image(&doom->screen, img,
			(SDL_Rect){ 0, 0, img->width, img->height },
			(SDL_Rect){ S_WIDTH - 335 + 50, 115, 220, 40 });
	}
	if (room->ceiling_texture
		&& room->ceiling_texture->type == RESSOURCE_TEXTURE)
	{
		img = room->ceiling_texture->data.texture;
		apply_image_to_image(&doom->screen, img,
			(SDL_Rect){ 0, 0, img->width, img->height },
			(SDL_Rect){ S_WIDTH - 335 + 50, 260, 220, 40 });
	}
}
