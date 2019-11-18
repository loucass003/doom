/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es.wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/17 22:41:53 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[0])
	{
		t_wall *wall = get_current_wall(editor);
		wall->texture = get_prev_texture(&doom->res_manager, wall->texture);
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[1])
	{
		t_wall *wall = get_current_wall(editor);
		wall->texture = get_next_texture(&doom->res_manager, wall->texture);
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[2])
	{
		t_wall *wall = get_current_wall(editor);
		wall->collisions = ((t_checkbox *)cmp)->value;
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[3])
	{
		t_wall *wall = get_current_wall(editor);
		wall->invisible = ((t_checkbox *)cmp)->value;
	}
	
	return (TRUE);
}

void			g_es_wall_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	t_wall *wall = get_current_wall(&doom->editor);
	if (!wall)
		return ;
	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 70 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[2])->value = wall->collisions;
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 170, y + 70 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[3])->value = wall->invisible;
	int i = -1;
	while (++i < 4)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_wall_render(t_gui *self, t_doom *doom)
{
	draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 135, 0xFFFFFFFF }, (t_pixel){ S_WIDTH - 335 + 160, 235, 0 });
	t_wall	*wall = get_current_wall(&doom->editor);
	if (!wall)
		return ;
	if (wall->texture && wall->texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img = wall->texture->data.texture;
		apply_image_to_image(&doom->screen, img, (SDL_Rect){ 0, 0, img->width, img->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 85, 220, 40 });
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			wall->texture->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 85, 220, 40});
		SDL_FreeSurface(text);
	}
}