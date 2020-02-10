/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 10:21:45 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/10 01:40:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"
#include "editor.h"

static void		draw_text_overlay(char *str, t_doom *doom)
{
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		str,
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(doom->main_context.image, text,
		(SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){ S_WIDTH - text->w - 15, 15, text->w, 20 });
	SDL_FreeSurface(text);
}

void			draw_transforms_type(t_editor *editor)
{
	const char		types[3][12] = {"TRANSLATION\0", "ROTATION\0", "SCALING\0"};
	const char		types_uv[2][12] = {"UV OFFSET\0", "UV REPEAT\0"};
	
	if (editor->doom->main_context.type != CTX_EDITOR)
		return ;
	if (editor->current_object != -1)
		draw_text_overlay((char *)types[(int)editor->object_transform_mode], editor->doom);
	else if (editor->wall_section != -1 || (editor->selected_floor_ceil != -1 && editor->current_room != -1))
		draw_text_overlay((char *)types_uv[(int)editor->uv_transform_mode], editor->doom);
}