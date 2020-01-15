/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_esc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 16:45:12 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/15 16:51:40 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"

void	g_esc_render(t_gui *self, t_doom *doom)
{
    SDL_Surface *text;
    const SDL_Color	color = {255, 255, 255, 0};
    
    text = TTF_RenderText_Blended(doom->fonts.helvetica, "SAUVEGARDE...", color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH_2 - text->w/2, S_HEIGHT_2 - text->h/2, text->w, text->h});
	SDL_FreeSurface(text);
}
