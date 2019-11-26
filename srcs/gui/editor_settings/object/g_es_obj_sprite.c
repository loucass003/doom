/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_sprite.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/26 15:22:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "sprite.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];

	if (cmp == editor->settings.guis_object[OBJECT_SPRITE].components->values[0])
	{
		t_sprite	*sprite = object->of.sprite;
		sprite->texture = get_prev_ressource(&doom->res_manager, sprite->texture, RESSOURCE_TEXTURE);
	}
	if (cmp == editor->settings.guis_object[OBJECT_SPRITE].components->values[1])
	{
		t_sprite	*sprite = object->of.sprite;
		sprite->texture = get_next_ressource(&doom->res_manager, sprite->texture, RESSOURCE_TEXTURE);
	}
	return (TRUE);
}

void			g_es_obj_sprite_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
	int			i;

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	i = -1;
	while (++i < 2)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_sprite_render(t_gui *self, t_doom *doom)
{
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	if (!object)
		return ;
	t_sprite	*sprite = object->of.sprite;
	if (sprite->texture && sprite->texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img = sprite->texture->data.texture;
		apply_image_to_image(&doom->screen, img, (SDL_Rect){ 0, 0, img->width, img->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 165, 220, 40 });
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			sprite->texture->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(text);
	}
}
