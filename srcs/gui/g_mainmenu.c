/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_mainmenu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:56:05 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/01 16:50:33 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
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
		if (!alloc_components(self, 3))
		return ;
//	printf("init %d\n", self->component_count);
	self->components[0] = create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, NULL, "PLAY");
	((t_button *)self->components[0])->color_default = 0xFF8d1506;
	((t_button *)self->components[0])->color_hover = 0;
	self->components[0]->perform_action = action_performed;
	self->components[1] = create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 20) / 2, 250, 50 }, NULL, "MAP EDITOR");
	((t_button *)self->components[1])->color_default = 0xFF8d1506;
	((t_button *)self->components[1])->color_hover = 0;
	self->components[1]->perform_action = action_performed;
	self->components[2] = create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT + 110) / 2, 250, 50 }, NULL, NULL);
	((t_button *)self->components[2])->color_default = 0xFF8d1506;
	((t_button *)self->components[2])->color_hover = 0;
	self->components[2]->perform_action = action_performed;
}

void	g_mainmenu_on_leave(t_gui *self, t_doom *doom)
{
	printf("HAHA\n");
	free_components(self);
}

void	g_mainmenu_render(t_gui *self, t_doom *doom)
{
	apply_surface(&doom->screen, doom->background, (SDL_Rect) {0, 0, doom->background->w, doom->background->h}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
	render_components(doom, self);
}
