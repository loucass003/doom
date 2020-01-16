/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_level.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 16:27:20 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/16 14:25:17 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"

void                play_game(t_doom *doom)
{
	init_player(doom);
    spawn_player(doom);
    if (doom->main_context.type == CTX_NORMAL)
    {
        obj_test(doom);
        init_bsp(doom);
        doom->player.entity.position = (t_vec3){ 0, 50, 0 };
    }
    else
        unselect_all(doom);
    doom->main_context.type = CTX_NORMAL;
    set_gui(doom, GUI_INGAME);
}

static t_bool		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[0])
	{
        doom->level.difficulty = D_EASY;
		doom->level.max_life = 4;
		doom->level.coeff_damage = 1.5;
		doom->level.coeff_regen = 1.5;
		doom->level.coeff_speed = 0.5;
        play_game(doom);
		return (FALSE);
	}
    if (cmp == doom->guis[doom->current_gui].components->values[1])
	{
        doom->level.difficulty = D_MEDIUM;
		doom->level.max_life = 2;
		doom->level.coeff_damage = 1;
		doom->level.coeff_regen = 1;
		doom->level.coeff_speed = 1;
        play_game(doom);
		return (FALSE);
	}
    if (cmp == doom->guis[doom->current_gui].components->values[2])
	{
        doom->level.difficulty = D_HARD;
		doom->level.max_life = 1;
		doom->level.coeff_damage =  0.5;
		doom->level.coeff_regen = 0.5;
		doom->level.coeff_speed = 1.5;
        play_game(doom);
		return (FALSE);
	}
	return (TRUE);
}

void	g_level_on_enter(t_gui *self, t_doom *doom)
{
	int		i;

    doom->mouse_focus = FALSE;
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, NULL, "EASY"));
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 20) / 2, 250, 50 }, NULL, "MEDIUM"));
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT + 110) / 2, 250, 50 }, NULL, "HARD"));
	
	i = -1;
	while (++i < self->components->len)
	{
		((t_button *)self->components->values[i])->color_default = 0xFF8d1506;
		((t_button *)self->components->values[i])->color_hover = 0;
		self->components->values[i]->perform_action = action_performed;
	}
}

void	g_level_on_leave(t_gui *self, t_doom *doom)
{
}

void	g_level_render(t_gui *self, t_doom *doom)
{
	render_components(doom, self);
}

