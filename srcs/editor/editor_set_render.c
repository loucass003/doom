/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_set_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:42:51 by louali            #+#    #+#             */
/*   Updated: 2020/03/11 16:39:05 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"
#include "door.h"

void		set_itemstack(t_renderable *r, t_object *object)
{
	t_itemstack	*itemstack;

	ft_bzero(r, sizeof(t_renderable));
	if (!(itemstack = ft_memalloc(sizeof(t_itemstack))))
		return ;
	ft_memcpy(itemstack, object->of.itemstack, sizeof(t_itemstack));
	create_itemstack_renderable(r, itemstack);
	r->position = editor_to_world(object->pos);
	r->position.y += r->scale.y * 0.5;
}

t_bool		set_sprite(t_renderable *r, t_object *object)
{
	ft_bzero(r, sizeof(t_renderable));
	if (!create_sprite_renderable(r, object->of.sprite))
		return (FALSE);
	r->position = editor_to_world(object->pos);
	return (TRUE);
}

void		set_entity(t_renderable *r, t_object *object, t_editor *editor)
{
	ft_bzero(r, sizeof(t_renderable));
	if (object->of.entity == ENTITY_ENEMY)
		create_enemy_renderable(editor->doom, r);
	else if (object->of.entity == ENTITY_BOSS)
		create_boss_renderable(editor->doom, r);
	r->of.data.entity->position = editor_to_world(object->pos);
	r->of.data.entity->position.y += r->of.data.entity->radius.y;
}

void		set_model(t_renderable *r, t_object *object)
{
	ft_bzero(r, sizeof(t_renderable));
	copy_renderable(object->of.model->data.model, r);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE;
	r->position = editor_to_world(object->pos);
	r->position.y += r->scale.y;
}

void		set_transpo(t_renderable *r, t_object *object, t_editor *editor)
{
	ft_bzero(r, sizeof(t_renderable));
	copy_renderable(editor->doom->res_manager.ressources->values[
		23]->data.model, r);
	r->of.type = RENDERABLE_TRANSPO;
	r->of.data.transpo = object->of.transpo;
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE;
	r->position = editor_to_world(object->pos);
}
