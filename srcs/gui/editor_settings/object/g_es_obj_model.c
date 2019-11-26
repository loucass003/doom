/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_model.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:00:53 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/26 15:30:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;
    t_object    *object = &doom->editor.objects->values[doom->editor.current_object];

    if (cmp == editor->settings.guis_object[OBJECT_MODEL].components->values[0])
	{
		object->of.model = get_prev_ressource(&doom->res_manager, object->of.model, RESSOURCE_MODEL);
	}
	if (cmp == editor->settings.guis_object[OBJECT_MODEL].components->values[1])
	{
		object->of.model = get_next_ressource(&doom->res_manager, object->of.model, RESSOURCE_MODEL);
	}
	return (TRUE);
}

void			g_es_obj_model_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
    int         i;

    append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	i = -1;
	while (++i < 2)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_model_render(t_gui *self, t_doom *doom)
{
    t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	if (!object)
		return ;
	t_ressource *model_r = object->of.model;
    if (model_r && model_r->type == RESSOURCE_MODEL)
    {
        SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			model_r->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(text);
    }
    // draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
