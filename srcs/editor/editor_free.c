/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:15:59 by louali            #+#    #+#             */
/*   Updated: 2020/02/13 15:16:43 by louali           ###   ########.fr       */
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
}

void		free_objects(t_objects **objects)
{
	int			i;
	t_object	*o;

	if (!*objects)
		return ;
	i = -1;
	while (++i < (*objects)->len)
	{
		o = &(*objects)->values[i];
		free_object(o);
	}
	ft_memdel((void **)objects);
}
