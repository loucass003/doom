/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_level.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 16:27:20 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/05 17:08:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"

void				play_game(t_doom *doom)
{
	doom->main_context.type = CTX_NORMAL;
	init_player(doom);
	spawn_player(doom);
	unselect_all(doom);
	leave_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	free_renderables(&doom->renderables, FALSE);
	editor_setmap(&doom->editor);
	set_gui(doom, GUI_INGAME);
}

static t_bool		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[0])
	{
		doom->level = (t_level){.difficulty = D_EASY, .max_life = 4,
			.coeff_damage = 1.5, .coeff_regen = 1.5, .coeff_speed = 0.5};
		play_game(doom);
		return (FALSE);
	}
	if (cmp == doom->guis[doom->current_gui].components->values[1])
	{
		doom->level = (t_level){.difficulty = D_MEDIUM, .max_life = 2,
			.coeff_damage = 1, .coeff_regen = 1, .coeff_speed = 1};
		play_game(doom);
		return (FALSE);
	}
	if (cmp == doom->guis[doom->current_gui].components->values[2])
	{
		doom->level = (t_level){.difficulty = D_HARD, .max_life = 1,
			.coeff_damage = 0.5, .coeff_regen = 0.5, .coeff_speed = 1.5};
		play_game(doom);
		return (FALSE);
	}
	return (TRUE);
}

void				g_level_on_enter(t_gui *self, t_doom *doom)
{
	int	i;

	doom->mouse_focus = FALSE;
	append_components_array(&self->components,
			create_button((SDL_Rect){ (S_WIDTH - 250) / 2,
				(S_HEIGHT - 150) / 2, 250, 50 }, NULL, "EASY"));
	append_components_array(&self->components,
			create_button((SDL_Rect){ (S_WIDTH - 250) / 2,
				(S_HEIGHT - 20) / 2, 250, 50 }, NULL, "MEDIUM"));
	append_components_array(&self->components,
			create_button((SDL_Rect){ (S_WIDTH - 250) / 2,
				(S_HEIGHT + 110) / 2, 250, 50 }, NULL, "HARD"));
	i = -1;
	while (++i < self->components->len)
	{
		((t_button *)self->components->values[i])->color_default = 0xFF8d1506;
		((t_button *)self->components->values[i])->color_hover = 0;
		self->components->values[i]->perform_action = action_performed;
	}
}

void				g_level_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;
}

void				g_level_render(t_gui *self, t_doom *doom)
{
	render_components(doom, self);
}
