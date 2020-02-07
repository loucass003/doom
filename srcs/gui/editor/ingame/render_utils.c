/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 10:21:45 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/07 11:36:23 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <limits.h>
#include "gui.h"
#include "octree.h"
#include <math.h>
#include "render.h"
#include "editor.h"
#include "threads.h"
#include "door.h"

void			draw_object_transform_type(t_editor *editor, t_gui *self)
{
	const char		types[3][12] = {"TRANSLATION\0", "ROTATION\0", "SCALING\0"};
	SDL_Surface		*text;
	
	(void)self;
	if (editor->current_object == -1
		|| editor->doom->main_context.type != CTX_EDITOR)
		return ;
	text = TTF_RenderText_Blended(editor->doom->fonts.helvetica,
		types[(int)editor->object_transform_mode],
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(editor->doom->main_context.image, text,
		(SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){ S_WIDTH - text->w - 15, 15, text->w, 20 });
	SDL_FreeSurface(text);
}