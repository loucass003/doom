/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_model.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:00:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 17:48:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool	action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*e = &doom->editor;
	t_object		*o;

	o = &e->objects->values[doom->editor.current_object];
	if (cmp == e->settings.guis_object[OBJECT_MODEL].components->values[0])
	{
		o->of.model = get_prev_ressource(&doom->res_manager,
			o->of.model, RESSOURCE_MODEL);
		if (o->r)
		{
			free_renderable(o->r, FALSE, FALSE);
			create_object_renderable((t_editor *)e, e->current_object, o->r);
		}
	}
	if (cmp == e->settings.guis_object[OBJECT_MODEL].components->values[1])
	{
		o->of.model = get_next_ressource(&doom->res_manager,
			o->of.model, RESSOURCE_MODEL);
		if (o->r)
		{
			free_renderable(o->r, FALSE, FALSE);
			create_object_renderable((t_editor *)e, e->current_object, o->r);
		}
	}
	return (TRUE);
}

void			g_es_obj_model_enter(t_gui *self, t_doom *doom)
{
	const int			x = S_WIDTH - 335;
	const int			y = 75 + 80;
	int					i;

	(void)doom;
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	i = -1;
	while (++i < 2)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_model_render(t_gui *self, t_doom *doom)
{
	t_object	*object;
	t_ressource *model_r;
	SDL_Surface *t;

	if (doom->editor.current_object == -1)
		return ;
	(void)self;
	object = &doom->editor.objects->values[doom->editor.current_object];
	model_r = object->of.model;
	if (model_r && model_r->type == RESSOURCE_MODEL && *model_r->display_name)
	{
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
			model_r->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(t);
	}
}
