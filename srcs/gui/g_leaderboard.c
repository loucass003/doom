/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_leaderboard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 14:40:41 by louali            #+#    #+#             */
/*   Updated: 2020/01/23 14:49:14 by louali           ###   ########.fr       */
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
	return (TRUE);
} 

void			g_leaderboard_on_enter(t_gui *self, t_doom *doom)
{
	doom->mouse_focus = FALSE;
	int		i;

	append_components_array(&self->components, create_button((SDL_Rect){ S_WIDTH - 210, S_HEIGHT - 60, 200, 50 }, NULL, "BACK"));

	i = -1;
	while (++i < self->components->len)
	{
		((t_button *)self->components->values[i])->color_default = 0xffd8cb99;
		((t_button *)self->components->values[i])->color_hover = 0xfffefece;
		((t_button *)self->components->values[i])->colortext = (SDL_Color){27, 30, 31, 0};
		self->components->values[i]->perform_action = action_performed;
	}
}

void			g_leaderboard_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;
}

void			g_leaderboard_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	(void)event;
	(void)self;
	(void)doom;
}

void			g_leaderboard_render(t_gui *self, t_doom *doom)
{
    t_img	*background = doom->res_manager.ressources->values[20]->data.texture;
    int        i;

	apply_image_to_image(&doom->screen, background, (SDL_Rect) {0, 0, background->width, background->height}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
    i = -1;
    while (++i < 5)
    {
        t_score score = doom->scores[i];
		if (*score.text)
		{
			if (i == 0)
			{
				SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
				score.text, (SDL_Color){255, 255, 255, 0});
				SDL_Surface *text2 = TTF_RenderText_Blended(doom->fonts.helvetica,
				ft_int_to_str(score.score).str, (SDL_Color){255, 255, 255, 0});
				apply_surface_blended(&doom->screen, text,
				(SDL_Rect){0, 0, text->w, text->h},
				(SDL_Rect){S_WIDTH- text->w - 365,
				263,
				text->w, text->h});
				apply_surface_blended(&doom->screen, text2,
				(SDL_Rect){0, 0, text2->w, text2->h},
				(SDL_Rect){S_WIDTH_2 - 200,
				263,
				text2->w, text2->h});
				SDL_FreeSurface(text);
				SDL_FreeSurface(text2);
			}
			else
			{
				SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
				score.text, (SDL_Color){255, 255, 255, 0});
				SDL_Surface *text2 = TTF_RenderText_Blended(doom->fonts.helvetica,
				ft_int_to_str(score.score).str, (SDL_Color){255, 255, 255, 0});
				apply_surface_blended(&doom->screen, text,
				(SDL_Rect){0, 0, text->w, text->h},
				(SDL_Rect){S_WIDTH- text->w - 365,
				282 + (i * 73),
				text->w, text->h});
				apply_surface_blended(&doom->screen, text2,
				(SDL_Rect){0, 0, text2->w, text2->h},
				(SDL_Rect){S_WIDTH_2 - 200,
				282 + (i * 73),
				text2->w, text2->h});
				SDL_FreeSurface(text);
				SDL_FreeSurface(text2);
			}
			

		}
    }

	render_components(doom, self);
}
