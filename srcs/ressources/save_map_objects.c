/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map_objects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:39:51 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:40:09 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "item.h"
#include "sprite.h"
#include "write_structs.h"

t_bool		write_object_model(t_ressource_manager *r, t_ressource *model)
{
	const int index = ressources_indexof(r->ressources, model);

	dp_write(r, &index, sizeof(int));
	return (TRUE);
}

t_bool		write_object_light(t_ressource_manager *r, int light_index)
{
	const t_light	light = r->doom->lights->values[light_index];

	dp_write(r, &light, sizeof(t_light));
	return (TRUE);
}

t_bool		write_object_transpo(t_ressource_manager *r, t_transpo *transpo)
{
	dp_write(r, transpo, sizeof(t_transpo));
	return (TRUE);
}

t_bool		write_object(t_ressource_manager *r, t_object *object)
{
	t_wr_object wr_object;

	wr_object = (t_wr_object) {
		.type = object->type,
		.pos = object->pos,
		.scale = object->scale,
		.rotation = object->rotation,
		.no_light = object->no_light
	};
	dp_write(r, &wr_object, sizeof(t_wr_object));
	if (object->type == OBJECT_ITEMSTACK)
		write_itemstack(r, object->of.itemstack);
	else if (object->type == OBJECT_SPRITE)
		write_sprite(r, object->of.sprite);
	else if (object->type == OBJECT_ENTITY)
		write_entity(r, object->of.entity);
	else if (object->type == OBJECT_MODEL)
		write_object_model(r, object->of.model);
	else if (object->type == OBJECT_LIGHT)
		write_object_light(r, object->of.light_index);
	else if (object->type == OBJECT_TRANSPO)
		write_object_transpo(r, object->of.transpo);
	return (TRUE);
}

t_bool		write_objects(t_ressource_manager *r)
{
	int		i;

	dp_write(r, &r->doom->editor.objects->len, sizeof(int));
	i = -1;
	while (++i < r->doom->editor.objects->len)
		if (!write_object(r, &r->doom->editor.objects->values[i]))
			return (FALSE);
	return (TRUE);
}
