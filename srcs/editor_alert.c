/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_alert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 14:37:21 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 17:08:45 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	set_alert_message(t_doom *doom)
{
	doom->editor.alert[0] = 0;
	doom->editor.alert[1] = 0;
	doom->editor.alert[2] = 0;
	doom->editor.alert[3] = 0;
}

void	write_alert_message(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;
	char			*close;
	char			*ok;
	char			*points;
	char			*lignes;
	char			*out;

	close = "you have to close the polygon";
	ok = "All your polygons are ok";
	points = "Pas sur ces points!!";
	lignes = "Non non non ! Pas sur les lignes";
	out = "Pas en dehors";
	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_itoa(doom->editor.floor), color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){376, 16, 20, 20});
	if (doom->editor.alert[0] == 0)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, close, color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	else if (doom->editor.alert[0] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, ok, color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[1] == 1 && doom->editor.click != 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, points, color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[2] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, lignes, color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[3] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, out, color);
		apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
}
