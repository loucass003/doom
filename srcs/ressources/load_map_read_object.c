/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_read_object.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:16:59 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:20:27 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "sprite.h"

t_bool		read_object_model(t_ressource_manager *r, t_ressource **model)
{
	int				model_index;
	t_ressource		*res;

	if (!io_memcpy(&r->reader, &model_index, sizeof(int)))
		return (FALSE);
	if (model_index < 0 || model_index >= r->ressources->len)
		return (FALSE);
	res = r->ressources->values[model_index];
	if (res->type != RESSOURCE_MODEL)
		return (FALSE);
	*model = res;
	return (TRUE);
}

t_bool		read_object_light(t_ressource_manager *r, int *light_index)
{
	t_light	wr_light;

	if (!io_memcpy(&r->reader, &wr_light, sizeof(t_light)))
		return (FALSE);
	*light_index = r->doom->lights->len;
	if (!append_lights_array(&r->doom->lights, wr_light))
		return (FALSE);
	return (TRUE);
}

t_bool		read_object_transpo(t_ressource_manager *r, t_transpo **t_addr)
{
	t_transpo	*transpo;

	if (!(transpo = malloc(sizeof(t_transpo))))
		return (FALSE);
	if (!io_memcpy(&r->reader, transpo, sizeof(t_transpo)))
		return (FALSE);
	*t_addr = transpo;
	return (TRUE);
}

t_bool		read_object_return(t_wr_object wr_object, t_ressource_manager *r,
	t_object *object)
{
	if (wr_object.type == OBJECT_ITEMSTACK
		&& !read_itemstack(r, &object->of.itemstack))
		return (FALSE);
	else if (wr_object.type == OBJECT_SPRITE
		&& !read_sprite(r, &object->of.sprite))
		return (FALSE);
	else if (wr_object.type == OBJECT_ENTITY
		&& !read_entity(r, &object->of.entity))
		return (FALSE);
	else if (wr_object.type == OBJECT_MODEL
		&& !read_object_model(r, &object->of.model))
		return (FALSE);
	else if (wr_object.type == OBJECT_LIGHT
		&& !read_object_light(r, &object->of.light_index))
		return (FALSE);
	else if (wr_object.type == OBJECT_TRANSPO
		&& !read_object_transpo(r, &object->of.transpo))
		return (FALSE);
	return (TRUE);
}

t_bool		read_object(t_ressource_manager *r, t_object *object)
{
	t_wr_object	wr_object;

	if (!io_memcpy(&r->reader, &wr_object, sizeof(t_wr_object)))
		return (FALSE);
	*object = (t_object) { .type = wr_object.type, .pos = wr_object.pos,
		.scale = wr_object.scale, .no_light = wr_object.no_light,
		.rotation = wr_object.rotation };
	return (read_object_return(wr_object, r, object));
}
