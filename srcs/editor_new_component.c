/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_component.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:04:43 by lloncham          #+#    #+#             */
/*   Updated: 2019/06/26 15:12:39 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			show_new_components(t_doom *doom)
{
	SDL_Surface		*image;
	t_obj			*obj;
	t_texture		*pin;

	if (doom->editor.set_start_pos[0] != 0)
	{
		pin = doom->icons;
		while (pin)
		{
			if (ft_strcmp(pin->name, "icons/pin.png") == 0 && doom->editor.set_start_pos[2] == doom->editor.floor)
			{
				image = pin->text;
				apply_surface(&doom->screen, image, (SDL_Rect){0, 0, 500, 500}
				, (SDL_Rect){doom->editor.set_start_pos[0] * 20 - 5
				, doom->editor.set_start_pos[1] * 20 - 20, 30, 30});
				break;
			}
			pin = pin->next;
		}
	}
	if (doom->editor.objet)
	{
		obj = doom->editor.obj;
		while (obj)
		{
			if (obj->floor == doom->editor.floor)
				apply_surface(&doom->screen, obj->img, (SDL_Rect){0, 0, obj->img->w, obj->img->w}
				, (SDL_Rect){obj->pos.x * 20 - 5, obj->pos.y * 20 - 20, 30, 30});
			obj = obj->next;
		}
	}
}
