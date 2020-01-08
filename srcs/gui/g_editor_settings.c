/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/08 13:11:38 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "doom.h"
#include "editor.h"
#include "gui.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	
}

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	editor_settings_guis(&doom->editor);
	editor_settings_update(&doom->editor);
}

void			g_editor_settings_on_leave(t_gui *self, t_doom *doom)
{
	set_gui_settings(&doom->editor, -1);
}

void			g_editor_settings_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	gui_events(doom, doom->editor.settings.guis, event, doom->editor.settings.current_gui);
	components_events(doom, doom->editor.settings.guis, event, doom->editor.settings.current_gui);
}

void			g_editor_settings_render(t_gui *self, t_doom *doom)
{
	if (!doom->editor.settings.open)
		return ;
	fill_rect(&doom->screen, (SDL_Rect){ S_WIDTH - 335, 75, 320, 550 }, 0xFF303030);
	draw_rect(&doom->screen, (SDL_Rect){ S_WIDTH - 335, 75, 320, 550 }, 0xFFFFFFFF);
	
	if (doom->editor.settings.current_gui != -1)
	{
		doom->editor.settings.guis[doom->editor.settings.current_gui]
			.render(&doom->editor.settings.guis[doom->editor.settings.current_gui], doom);
		
		render_components(doom, &doom->editor.settings.guis[doom->editor.settings.current_gui]);
	}
	render_components(doom, self);
}
