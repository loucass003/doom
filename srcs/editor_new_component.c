/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_component.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:04:43 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/17 15:32:08 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			show_new_components(t_doom *doom)
{
	SDL_Surface		*image;

	if (doom->editor.set_start_pos[0] != 0)
	{
		image = IMG_Load("pin.png");
		apply_surface(&doom->screen, image, (SDL_Rect){0, 0, 500, 500}
			, (SDL_Rect){doom->editor.set_start_pos[0] * 20 - 5
			, doom->editor.set_start_pos[1] * 20 - 20, 30, 30});
	}
}
