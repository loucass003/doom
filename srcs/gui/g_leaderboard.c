/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_leaderboard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 14:40:41 by louali            #+#    #+#             */
/*   Updated: 2020/01/21 16:24:30 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
    if (cmp == doom->guis[doom->current_gui].components->values[0])
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
} 

// void			g_editor_button(t_gui *self, t_doom *doom)
// {
// }

void			g_leaderboard_on_enter(t_gui *self, t_doom *doom)
{
	doom->mouse_focus = FALSE;
	int		i;

	// enter_gui(doom, doom->guis, GUI_LEADERBOARD);
	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, NULL, "Main Menu"));

	i = -1;
	while (++i < self->components->len)
	{
		((t_button *)self->components->values[i])->color_default = 0xFF8d1506;
		((t_button *)self->components->values[i])->color_hover = 0;
		self->components->values[i]->perform_action = action_performed;
	}
}

void			g_leaderboard_on_leave(t_gui *self, t_doom *doom)
{
}

void			g_leaderboard_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
}

void			g_leaderboard_render(t_gui *self, t_doom *doom)
{
    t_img	*background = doom->res_manager.ressources->values[0]->data.texture;
    int        i;

	apply_image_to_image(&doom->screen, background, (SDL_Rect) {0, 0, background->width, background->height}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
    i = -1;
    while (++i < doom->scores->len)
    {
        t_score score = doom->scores->values[i];
        SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
        score.text, (SDL_Color){255, 255, 255, 0});
        apply_surface_blended(&doom->screen, text,
        (SDL_Rect){0, 0, text->w, text->h},
        (SDL_Rect){S_WIDTH_2 - 100,
        100 + (i * (text->h + 5)),
        text->w, text->h});
        SDL_FreeSurface(text);
    }

	render_components(doom, self);
}
