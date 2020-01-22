/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_global.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 11:49:48 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

    if (cmp == editor->settings.guis[ES_GUI_GLOBAL].components->values[0])
    {
        doom->skybox_enabled = ((t_checkbox *)cmp)->value;
    }
	return (TRUE);
}

void			g_es_global_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

    append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 40 }, "SKYBOX"));
	((t_checkbox *)self->components->values[0])->value = doom->skybox_enabled;
	// append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 40, 40, 40 }, NULL, "<"));
	// append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 40, 40, 40 }, NULL, ">"));
	// append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 120, 40, 40 }, NULL, "<"));
	// append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 120, 40, 40 }, NULL, ">"));
	int i = -1;
	while (++i < 1)
	 	self->components->values[i]->perform_action = action_performed;
}

void			g_es_global_render(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;	
}
