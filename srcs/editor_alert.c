/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_alert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 14:37:21 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/07 14:45:42 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	set_alert_message(t_doom *doom)
{
	doom->editor.alert[0] = 0;
	doom->editor.alert[1] = 0;
	doom->editor.alert[2] = 0;
}

void	write_alert_message(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	if (doom->editor.alert[0] == 0)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "you have to close the polygon", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	else if (doom->editor.alert[0] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "All your polygons are ok", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[1] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Pas sur ces points!!", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[2] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Non non non ! Pas sur les lignes", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
}