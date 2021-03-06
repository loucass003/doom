/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 18:16:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool	action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor			*editor = &doom->editor;
	t_player_spawn_data		*s_data;

	s_data = &doom->player.spawn_data;
	if (cmp == editor->settings.guis[ES_GUI_PLAYER].components->values[0])
	{
		s_data->rotation.x = 0;
		if (!((t_textfield *)cmp)->error)
			s_data->rotation.x = ((t_textfield *)cmp)->value * (M_PI / 180.);
	}
	if (cmp == editor->settings.guis[ES_GUI_PLAYER].components->values[1])
	{
		s_data->rotation.y = 0;
		if (!((t_textfield *)cmp)->error)
			s_data->rotation.y = ((t_textfield *)cmp)->value * (M_PI / 180.);
	}
	if (cmp == editor->settings.guis[ES_GUI_PLAYER].components->values[2])
	{
		s_data->rotation.z = 0;
		if (!((t_textfield *)cmp)->error)
			s_data->rotation.z = ((t_textfield *)cmp)->value * (M_PI / 180.);
	}
	return (TRUE);
}

void			g_es_player_enter(t_gui *self, t_doom *doom)
{
	const int		x = S_WIDTH - 335;
	const int		y = 85;
	t_int_str		istr;
	t_components	*cmps;

	cmps = self->components;
	append_components_array(&cmps,
		create_textfield((SDL_Rect){x + 10, y + 30, 95, 30},
			"ROTATION X", TRUE));
	istr = ft_int_to_str(doom->player.spawn_data.rotation.x * (180. / M_PI));
	set_text_value((t_textfield *)cmps->values[0], istr.str, istr.len);
	cmps->values[0]->perform_action = action_performed;
	append_components_array(&cmps,
		create_textfield((SDL_Rect){x + 110, y + 30, 95, 30},
			"ROTATION Y", TRUE));
	istr = ft_int_to_str(doom->player.spawn_data.rotation.y * (180. / M_PI));
	set_text_value((t_textfield *)cmps->values[1], istr.str, istr.len);
	cmps->values[1]->perform_action = action_performed;
	append_components_array(&cmps,
		create_textfield((SDL_Rect){x + 210, y + 30, 95, 30},
			"ROTATION Z", TRUE));
	istr = ft_int_to_str(doom->player.spawn_data.rotation.z * (180. / M_PI));
	set_text_value((t_textfield *)cmps->values[2], istr.str, istr.len);
	cmps->values[2]->perform_action = action_performed;
}

void			g_es_player_render(t_gui *self, t_doom *doom)
{
	SDL_Surface *text;

	(void)self;
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		"PLAYER ROTATION", (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text,
		(SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 85, 220, 20});
	SDL_FreeSurface(text);
}
