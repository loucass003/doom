/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_render.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 15:15:34 by louali            #+#    #+#             */
/*   Updated: 2020/02/14 16:16:11 by louali           ###   ########.fr       */
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

t_bool		create_object_renderable(t_editor *editor, int object_index, t_renderable *r)
{
	t_object	*object = &editor->objects->values[object_index];

	if (object->type == OBJECT_ITEMSTACK)
	{
		ft_bzero(r, sizeof(t_renderable));
		t_itemstack		*itemstack = object->of.itemstack;
		
		create_itemstack_renderable(r, itemstack->of, itemstack->amount);
		r->position = editor_to_world(object->pos);
		r->position.y += r->scale.y * 0.5;
	}
	else if (object->type == OBJECT_SPRITE)
	{
		ft_bzero(r, sizeof(t_renderable));
		if (!create_sprite_renderable(r, object->of.sprite))
			return (FALSE);
		r->position = editor_to_world(object->pos);
	}
	else if (object->type == OBJECT_ENTITY)
	{
		ft_bzero(r, sizeof(t_renderable));
		if (object->of.entity == ENTITY_ENEMY)
			create_enemy_renderable(editor->doom, r);
		else if (object->of.entity == ENTITY_BOSS)
			create_boss_renderable(editor->doom, r);
		r->of.data.entity->position = editor_to_world(object->pos);
		r->of.data.entity->position.y += r->of.data.entity->radius.y;
	}
	else if (object->type == OBJECT_MODEL)
	{
		ft_bzero(r, sizeof(t_renderable));
		copy_renderable(object->of.model->data.model, r);
		r->scale = (t_vec3){ 1, 1, 1 };
		r->dirty = TRUE;
		r->position = editor_to_world(object->pos);
		r->position.y += r->scale.y;
	}
	else if (object->type == OBJECT_TRANSPO)
	{
		ft_bzero(r, sizeof(t_renderable));
		copy_renderable(editor->doom->res_manager.ressources->values[23]->data.model, r);
		r->of.type = RENDERABLE_TRANSPO;
		r->of.data.transpo = object->of.transpo;
		r->scale = (t_vec3){ 1, 1, 1 };
		r->dirty = TRUE;
		r->position = editor_to_world(object->pos);
	}
	else if (object->type == OBJECT_LIGHT)
	{
		t_light		*light = &editor->doom->lights->values[object->of.light_index];

		light->position = editor_to_world(object->pos);
		ft_bzero(r, sizeof(t_renderable));
		create_ellipsoid(editor->doom, r, (t_vec2){ 12, 12 }, (t_vec3){ 1, 1, 1 });
		r->materials->values[0].material_color = 0xFFF0E68C;
		r->position = light->position;
		object->scale = (t_vec3){ 0.2, 0.2, 0.2 };
		r->visible = light->model_visible;
		r->no_light = TRUE;
		r->no_collision = TRUE;
		r->scale = object->scale;
	}
	if (r->has_hitbox)
		r->show_hitbox = TRUE;
	if (object->scale.x == 0 && object->scale.y == 0 && object->scale.z == 0)
		object->scale = r->scale;
	else
		r->scale = object->scale;
	if (object->rotation.x == 0 && object->rotation.y == 0 && object->rotation.z == 0)
		object->rotation = r->rotation;
	else
		r->rotation = object->rotation;
	if (object->type == OBJECT_ENTITY)
		r->of.data.entity->rotation = object->rotation;
	r->object_index = object_index;
	r->no_light = object->no_light;
	object->r = r;
	return (TRUE);
}

void			default_renderables(t_doom *doom)
{
	t_renderable	skybox;
	t_renderable	r;

	create_cube(doom, &skybox, TRUE);
	skybox.materials->values[0].texture_map_set = TRUE;
	skybox.materials->values[0].texture_map =
		doom->res_manager.ressources->values[9]->data.texture;
	skybox.scale = (t_vec3){FAR_CULL, FAR_CULL, FAR_CULL};
	skybox.no_light = TRUE;
	skybox.no_collision = TRUE;
	doom->skybox_index = doom->renderables->len;
	append_renderables_array(&doom->renderables, skybox);
	create_player(&r, doom);
	append_renderables_array(&doom->renderables, r);
}
