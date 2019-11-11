/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/11 00:26:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	
}

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	printf("ENTER\n");
}

void			g_editor_settings_on_leave(t_gui *self, t_doom *doom)
{
	printf("LEAVE\n");
}

void			g_editor_settings_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	
}

void			g_editor_settings_render(t_gui *self, t_doom *doom)
{
	render_components(doom, self);
}
