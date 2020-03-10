/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 21:23:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void			g_es_wall_definition_cmps(t_gui *self, int x, int y,
	t_wall_section *ws)
{
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	append_components_array(&self->components,
		create_select((SDL_Rect){ x + 10, y + 60, 300, 30}, "WALL TYPE"));
	((t_select *)self->components->values[2])->items =
		create_select_items_array(2);
	append_select_items_array(&((t_select *)self->components->values[2])->items,
		(t_select_item){ .name = "WALL", .value = WS_WALL });
	append_select_items_array(&((t_select *)self->components->values[2])->items,
		(t_select_item){ .name = "DOOR", .value = WS_DOOR });
	((t_select *)self->components->values[2])->selected_item =
		select_items_indexof(((t_select *)self->components->values[2])->items,
			ws->type);
}

void			g_es_wall_settings_cmps(t_gui *self, t_doom *doom, int p[2],
	t_wall_section *ws)
{
	append_components_array(&self->components,
		create_button((SDL_Rect){ p[0] + 10, p[1] + 100, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ p[0] + 270, p[1] + 100, 40, 40 }, NULL, ">"));
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ p[0] + 10, p[1] + 170 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[5])->value = ws->collisions;
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ p[0] + 170, p[1] + 170 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[6])->value = ws->invisible;
	append_components_array(&self->components,
		create_select((SDL_Rect){ p[0] + 10, p[1] + 190, 300, 30},
			"NORMAL TYPE"));
	((t_select *)self->components->values[7])->items = normals_types();
	((t_select *)self->components->values[7])->selected_item =
		select_items_indexof(((t_select *)self->components->values[7])->items,
			ws->normal_type);
}

void			g_es_wall_enter(t_gui *self, t_doom *doom)
{
	const int		x = S_WIDTH - 335;
	const int		y = 75;
	int				i;
	t_wall_section	*ws;
	t_wall			*wall;

	wall = get_current_wall(&doom->editor);
	if (!wall || doom->editor.wall_section == -1)
		return ;
	ws = &wall->wall_sections->values[doom->editor.wall_section];
	g_es_wall_definition_cmps(self, x, y, ws);
	g_es_wall_settings_cmps(self, doom, (int[2]){x, y}, ws);
	i = -1;
	while (++i < 8)
		self->components->values[i]->perform_action =
			g_es_wall_action_performed;
	g_es_wall_components_visibility(self->components, ws);
}

void			g_es_wall_render_text(t_doom *doom)
{
	SDL_Surface		*t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica, "Section :",
		(SDL_Color){255, 0, 0, 0});
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 85, 180, 40});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(doom->
		editor.wall_section).str, (SDL_Color){255, 0, 0, 0});
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50 + 170, 85, 40, 40});
	SDL_FreeSurface(t);
}

void			g_es_wall_render(t_gui *self, t_doom *doom)
{
	t_wall			*wall;
	t_wall_section	*ws;
	SDL_Surface		*t;
	t_img			*i;

	(void)self;
	wall = get_current_wall(&doom->editor);
	if (!wall || doom->editor.wall_section == -1)
		return ;
	ws = &wall->wall_sections->values[doom->editor.wall_section];
	g_es_wall_render_text(doom);
	if (ws->type == WS_WALL && ws->texture
		&& ws->texture->type == RESSOURCE_TEXTURE && *ws->texture->display_name)
	{
		i = ws->texture->data.texture;
		apply_image_to_image(&doom->screen, i,
			(SDL_Rect){ 0, 0, i->width, i->height },
			(SDL_Rect){ S_WIDTH - 335 + 50, 175, 220, 40 });
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
			ws->texture->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 175, 220, 40});
		SDL_FreeSurface(t);
	}
}
