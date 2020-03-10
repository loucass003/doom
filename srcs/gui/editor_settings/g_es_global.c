/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_global.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 18:01:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool	action_performed(t_component *cmp, t_doom *doom)
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
	const int			x = S_WIDTH - 335;
	const int			y = 75;
	int					i;

	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 10, y + 40 }, "SKYBOX"));
	((t_checkbox *)self->components->values[0])->value = doom->skybox_enabled;
	i = -1;
	while (++i < 1)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_global_render(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;
}
