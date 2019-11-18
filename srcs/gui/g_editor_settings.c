/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/17 22:48:40 by llelievr         ###   ########.fr       */
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
}

void			g_editor_settings_on_leave(t_gui *self, t_doom *doom)
{
	set_gui_settings(&doom->editor, -1);
}

void			g_editor_settings_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	t_gui	*gui;
	int		i;
	
	if (doom->editor.settings.current_gui != -1)
	{
		gui = &doom->editor.settings.guis[doom->editor.settings.current_gui];
		i = -1;
		while (++i < gui->components->len)
		{
			if (gui->components->values[i]->enabled 
				&& gui->components->values[i]->on_event
				&& !gui->components->values[i]->on_event(gui->components->values[i],
					event, doom))
				break;
		}
	}
}

void			g_editor_settings_render(t_gui *self, t_doom *doom)
{
	if (!doom->editor.settings.open || !doom->editor.settings.visible)
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
