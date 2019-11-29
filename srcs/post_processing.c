/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/29 22:13:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

t_bool			post_process_map(t_doom *doom)
{
	/* int		i;

	i = -1;
	while (++i < doom->polygons->len)
		post_process_polygon(doom->polygons->polygons + i);
	i = -1;
	while (++i < doom->objects->len)
		post_process_obj(doom->objects->objs + i);
	init_lightning(doom); */

	int				i;
	t_renderable	*r;

	i = -1;
	while (++i < doom->renderables->len)
	{
		r = &doom->renderables->values[i];
		 if (r->of.type == RENDERABLE_OBJ)
			post_process_obj(doom, r);
	}
	return (TRUE);
}
