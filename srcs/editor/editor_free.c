/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:15:59 by louali            #+#    #+#             */
/*   Updated: 2020/03/08 20:13:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void		free_object(t_object *o)
{
	if (o->type == OBJECT_SPRITE)
		ft_memdel((void **)&o->of.sprite);
	else if (o->type == OBJECT_ITEMSTACK)
		free_itemstack(&o->of.itemstack);
	else if (o->type == OBJECT_TRANSPO)
		ft_memdel((void **)&o->of.transpo);
}

void		free_objects(t_objects **objects, t_bool force)
{
	int			i;
	t_object	*o;

	if (!*objects)
		return ;
	i = -1;
	while (++i < (*objects)->len)
	{
		o = &(*objects)->values[i];
		if (!o->r || force)
			free_object(o);
	}
	ft_memdel((void **)objects);
}
