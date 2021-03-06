/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 18:58:18 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "doom.h"
#include "editor.h"
#include "gui.h"

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	(void)self;
	editor_settings_guis(&doom->editor);
	editor_settings_update(&doom->editor);
}

void			g_editor_settings_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	set_gui_settings(&doom->editor, -1);
}

void			g_editor_settings_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	(void)self;
	gui_events(doom, doom->editor.settings.guis,
		event, doom->editor.settings.current_gui);
	components_events(doom, doom->editor.settings.guis,
		event, doom->editor.settings.current_gui);
}

void			g_editor_settings_render(t_gui *self, t_doom *doom)
{
	if (!doom->editor.settings.open)
		return ;
	fill_rect(&doom->screen, (SDL_Rect)
		{ S_WIDTH - 335, 75, 320, 550 }, 0xFF303030);
	draw_rect(&doom->screen, (SDL_Rect)
		{ S_WIDTH - 335, 75, 320, 550 }, 0xFFFFFFFF);
	if (doom->editor.settings.current_gui != -1)
	{
		doom->editor.settings.guis[doom->editor.settings.current_gui]
			.render(&doom->editor.settings
				.guis[doom->editor.settings.current_gui], doom);
		render_components(doom,
			&doom->editor.settings.guis[doom->editor.settings.current_gui]);
	}
	render_components(doom, self);
}
