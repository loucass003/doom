/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_sprite.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 17:20:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "sprite.h"
#include "doom.h"

static t_bool				action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*e = &doom->editor;
	t_object		*object;
	t_sprite		*sprite;

	object = &doom->editor.objects->values[doom->editor.current_object];
	sprite = object->of.sprite;
	if (cmp == e->settings.guis_object[OBJECT_SPRITE].components->values[0])
		sprite->texture = get_prev_ressource(&doom->res_manager,
			sprite->texture, RESSOURCE_TEXTURE);
	if (cmp == e->settings.guis_object[OBJECT_SPRITE].components->values[1])
		sprite->texture = get_next_ressource(&doom->res_manager,
			sprite->texture, RESSOURCE_TEXTURE);
	if (cmp == e->settings.guis_object[OBJECT_SPRITE].components->values[2])
		sprite->always_facing_player = ((t_checkbox *)cmp)->value;
	if (object->r)
	{
		free_renderable(object->r, FALSE, TRUE, FALSE);
		create_object_renderable((t_editor *)e, e->current_object, object->r);
	}
	return (TRUE);
}

void						g_es_obj_sprite_enter(t_gui *self, t_doom *doom)
{
	t_object		*object;
	const int		x = S_WIDTH - 335;
	const int		y = 75 + 80;
	int				i;

	object = &doom->editor.objects->values[doom->editor.current_object];
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 10, y + 70}, "Facing player"));
	((t_checkbox *)self->components->values[2])->value =
		object->of.sprite->always_facing_player;
	i = -1;
	while (++i < 3)
		self->components->values[i]->perform_action = action_performed;
}

void						g_es_obj_sprite_render(t_gui *self, t_doom *doom)
{
	t_object		*object;
	t_img			*img;
	t_sprite		*sprite;
	SDL_Surface		*t;

	(void)self;
	object = &doom->editor.objects->values[doom->editor.current_object];
	if (!object)
		return ;
	sprite = object->of.sprite;
	if (sprite->texture && sprite->texture->type == RESSOURCE_TEXTURE
		&& *sprite->texture->display_name)
	{
		img = sprite->texture->data.texture;
		apply_image_to_image(&doom->screen, img,
			(SDL_Rect){ 0, 0, img->width, img->height },
			(SDL_Rect){ S_WIDTH - 335 + 50, 165, 220, 40 });
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
			sprite->texture->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(t);
	}
}
