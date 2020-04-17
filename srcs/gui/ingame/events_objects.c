/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 11:32:00 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/17 22:12:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"
#include "editor.h"

void		unselect_all(t_doom *doom)
{
	if (doom->editor.current_object != -1)
		doom->editor.objects->values[doom->editor.current_object].r
			->show_hitbox = FALSE;
	doom->editor.object_transform_mode = OT_MODE_TRANSLATION;
	doom->editor.current_object = -1;
	doom->editor.wall_section = -1;
	select_room(&doom->editor, -1);
}

void		transform_object(t_doom *doom, t_object *object, t_vec3 add)
{
	t_light *light;

	object->pos = ft_vec3_add(object->pos, add);
	if (object->r)
	{
		object->r->position = editor_to_world(object->pos);
		if (object->type == OBJECT_ENTITY)
		{
			object->r->of.data.entity->position = editor_to_world(object->pos);
			object->r->of.data.entity->position.y += add.y;
			object->r->of.data.entity->position.y += object->r->of.data.entity
				->radius.y;
		}
		else if (object->type == OBJECT_ITEMSTACK)
			object->r->position.y += object->r->scale.y * 0.5;
		else if (object->type == OBJECT_MODEL)
			object->r->position.y += object->r->scale.y;
		object->r->position.y += add.y;
		if (object->type == OBJECT_LIGHT)
		{
			light = &doom->lights->values[object->of.light_index];
			light->position = object->r->position;
		}
		object->r->dirty = TRUE;
	}
}

void		rotation_mode(t_object *object, t_vec3 add)
{
	object->rotation = ft_vec3_add(object->rotation, add);
	if (object->r)
	{
		if (object->r->of.type == RENDERABLE_ENTITY)
			object->r->of.data.entity->rotation = object->rotation;
		else
			object->r->rotation = object->rotation;
		object->r->dirty = TRUE;
	}
}

void		object_mode(t_doom *doom, t_vec3 add)
{
	t_object *object;

	object = &doom->editor.objects->values[doom->editor.current_object];
	if (doom->editor.object_transform_mode == OT_MODE_TRANSLATION)
	{
		if (add.x != 0 || add.y != 0 || add.z != 0)
			transform_object(doom, object, add);
	}
	else if (doom->editor.object_transform_mode == OT_MODE_ROTATION)
		rotation_mode(object, add);
	else if (doom->editor.object_transform_mode == OT_MODE_SCALE)
	{
		object->scale = ft_vec3_add(object->scale, ft_vec3_mul_s(add, 0.1));
		if (object->r)
		{
			object->r->scale = object->scale;
			object->r->dirty = TRUE;
		}
	}
}

void		object_events(t_doom *doom, SDL_Event *event)
{
	t_vec3				add;
	const SDL_Scancode	key = event->key.keysym.scancode;

	add = (t_vec3){ 0, 0, 0 };
	if (key == SDL_SCANCODE_L)
	{
		doom->editor.object_transform_mode++;
		if (doom->editor.object_transform_mode == 3)
			doom->editor.object_transform_mode = 0;
	}
	else if (key == SDL_SCANCODE_P)
	{
		doom->editor.object_transform_mode--;
		if ((int)doom->editor.object_transform_mode == -1)
			doom->editor.object_transform_mode = 2;
	}
	if (key == SDL_SCANCODE_2 || key == SDL_SCANCODE_1)
		add.y = 0.1 * (key == SDL_SCANCODE_2 ? 1 : -1);
	if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_RIGHT)
		add.x = 0.1 * (key == SDL_SCANCODE_RIGHT ? 1 : -1);
	if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN)
		add.z = 0.1 * (key == SDL_SCANCODE_UP ? 1 : -1);
	object_mode(doom, add);
}
