/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_mainmenu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:56:05 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/22 13:37:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_INGAME);
	}
	else if (cmp == doom->guis[doom->current_gui].components[1])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_mainmenu_on_enter(t_gui *self, t_doom *doom)
{
	if (!alloc_components(self, 2))
		return ;
//	printf("init %d\n", self->component_count);
	self->components[0] = create_button((SDL_Rect){ 5, 5, 200, 20 }, NULL);
	self->components[0]->perform_action = action_performed;
	self->components[1] = create_button((SDL_Rect){ 5, 30, 200, 20 }, NULL);
	((t_button *)self->components[1])->color_default = 0xFF;
	((t_button *)self->components[1])->color_hover = 0xFF0000;
	self->components[1]->perform_action = action_performed;
}

void	g_mainmenu_on_leave(t_gui *self, t_doom *doom)
{
	free_components(self);
}

void	g_mainmenu_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF00);
	render_components(doom, self);
}
