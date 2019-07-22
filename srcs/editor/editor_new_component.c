/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_component.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:04:43 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/22 15:22:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			show_new_components(t_doom *doom)
{
	t_objects		*obj;
	t_texture		*pin;
	int				i;

	if (doom->editor.set_start_pos[0] != 0)
	{
		pin = doom->icons;
		while (pin)
		{
			if (ft_strcmp(pin->name, "icons/pin.png") == 0)
			{
				apply_surface(&doom->screen, pin->text, (SDL_Rect){0, 0, 500, 500}
				, (SDL_Rect){doom->editor.set_start_pos[0] * 20 - 5
				, doom->editor.set_start_pos[1] * 20 - 20, 30, 30});
				break;
			}
			pin = pin->next;
		}
	}
	if (doom->editor.objet)
	{
		obj = doom->editor.objects;
		i = -1;
		while (++i < obj->len)
		{
			apply_surface(&doom->screen, obj->values[i].img, (SDL_Rect){0, 0, obj->values[i].img->w, obj->values[i].img->h},
				(SDL_Rect){obj->values[i].pos.x * 20 - 5, obj->values[i].pos.y * 20 - 20, 30, 30});
		}
	}
}
