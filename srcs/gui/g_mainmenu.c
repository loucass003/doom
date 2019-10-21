/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_mainmenu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:56:05 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/20 00:01:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[0])
		set_gui(doom, GUI_INGAME);
	else if (cmp == doom->guis[doom->current_gui].components->values[1])
		set_gui(doom, GUI_EDITOR);
}

void	g_mainmenu_on_enter(t_gui *self, t_doom *doom)
{
	int		i;

	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, NULL, "PLAY"));
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 20) / 2, 250, 50 }, NULL, "MAP EDITOR"));
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT + 110) / 2, 250, 50 }, NULL, NULL));
	
	i = -1;
	while (++i < self->components->len)
	{
		((t_button *)self->components->values[i])->color_default = 0xFF8d1506;
		((t_button *)self->components->values[i])->color_hover = 0;
		self->components->values[i]->perform_action = action_performed;
	}
}

void	g_mainmenu_on_leave(t_gui *self, t_doom *doom)
{
}

void	g_mainmenu_render(t_gui *self, t_doom *doom)
{
	apply_surface(&doom->screen, doom->background, (SDL_Rect) {0, 0, doom->background->w, doom->background->h}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
	render_components(doom, self);
}
