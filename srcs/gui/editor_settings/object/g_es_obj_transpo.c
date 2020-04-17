/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_transpo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 11:23:57 by louali            #+#    #+#             */
/*   Updated: 2020/03/09 17:16:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor		*editor = &doom->editor;
	t_object			*object;
	t_transpo			*transpo;

	object = &doom->editor.objects->values[doom->editor.current_object];
	transpo = object->of.transpo;
	if (cmp == editor->settings.guis_object[OBJECT_TRANSPO]
		.components->values[0])
		prev_transpo(transpo, doom);
	if (cmp == editor->settings.guis_object[OBJECT_TRANSPO]
		.components->values[1])
		next_transpo(transpo, doom);
	return (TRUE);
}

void					g_es_obj_transpo_enter(t_gui *self, t_doom *doom)
{
	const int	x = S_WIDTH - 335;
	const int	y = 75 + 80;
	int			i;

	(void)doom;
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components,
		create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	i = -1;
	while (++i < 2)
		self->components->values[i]->perform_action = action_performed;
}

void					g_es_obj_transpo_render(t_gui *self, t_doom *doom)
{
	t_object		*object;
	t_object		*connect;
	SDL_Surface		*t;

	(void)self;
	object = &doom->editor.objects->values[doom->editor.current_object];
	connect = &doom->editor.objects->values[object->of.transpo->connected];
	if (object->of.transpo->connected == doom->editor.current_object)
	{
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
		"Not connected", (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
	}
	else
	{
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_strcat(ft_strcat(ft_int_to_str(connect->pos.x).str, ","),
			ft_int_to_str(connect->pos.z).str), (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
	}
	SDL_FreeSurface(t);
}
