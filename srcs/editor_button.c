/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_button.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:02:40 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/17 13:15:20 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			write_text_button(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica
		, "set start pos", color);
	apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 165, 55, text->w, text->h});
	text = TTF_RenderText_Blended(doom->fonts.helvetica, "sup line", color);
	apply_surface_blended(&doom->screen, text
		, (SDL_Rect){0, 0, text->w, text->h}
		, (SDL_Rect){S_WIDTH - 165, 105, text->w, text->h});
}