/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_transpo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 11:23:57 by louali            #+#    #+#             */
/*   Updated: 2020/02/11 13:46:29 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*editor = &doom->editor;
    t_objects		*objects = doom->editor.objects;
    t_object		*object = &doom->editor.objects->values[doom->editor.current_object];
	t_transpo		*transpo = object->of.transpo;

    if (cmp == editor->settings.guis_object[OBJECT_TRANSPO].components->values[0])
	{
		if (transpo->connected == 0)
			transpo->connected = objects->len;
		transpo->connected--;
		while (objects->values[transpo->connected].type != OBJECT_TRANSPO)
		{
			if (transpo->connected == 0)
				transpo->connected = objects->len;
			transpo->connected--;
		}
	}
	if (cmp == editor->settings.guis_object[OBJECT_TRANSPO].components->values[1])
	{
		if (transpo->connected == objects->len - 1)
			transpo->connected = 0;
		else
			transpo->connected++;
		while (objects->values[transpo->connected].type != OBJECT_TRANSPO)
		{
			if (transpo->connected == objects->len - 1)
				transpo->connected = 0;
			else
				transpo->connected++;
		}
	}
	if (object->r)
		create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
	return (TRUE);
}

void			g_es_obj_transpo_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
    int         i;

	(void)doom;
    append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	i = -1;
	while (++i < 2)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_transpo_render(t_gui *self, t_doom *doom)
{
	(void)self;
    t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
    t_object	*connect = &doom->editor.objects->values[object->of.transpo->connected];

	if (!object)
		return ;
	if (object->of.transpo->connected == doom->editor.current_object)
	{
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
		"Not connected", (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(text);
	}
	else
	{
		char *s = ft_strcat(ft_strcat(ft_int_to_str(connect->pos.x).str, ","), ft_int_to_str(connect->pos.z).str);
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
		s, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 40});
		SDL_FreeSurface(text);		
	}
}
