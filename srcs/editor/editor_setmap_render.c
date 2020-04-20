/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_render.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 15:15:34 by louali            #+#    #+#             */
/*   Updated: 2020/04/19 22:43:47 by llelievr         ###   ########.fr       */
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
#include "player.h"

void		set_light(t_renderable *r, t_object *object, t_editor *editor)
{
	t_light		*light;

	light = &editor->doom->lights->values[object->of.light_index];
	light->position = editor_to_world(object->pos);
	ft_bzero(r, sizeof(t_renderable));
	create_ellipsoid(editor->doom, r, (t_vec2){ 12, 12 },
		(t_vec3){ 1, 1, 1 });
	r->materials->values[0].material_color = 0xFFF0E68C;
	r->position = light->position;
	object->scale = (t_vec3){ 0.2, 0.2, 0.2 };
	r->visible = light->model_visible;
	r->no_light = TRUE;
	r->no_collision = TRUE;
	r->scale = object->scale;
}

t_bool		set_objects(t_object *object, t_renderable *r, t_editor *editor)
{
	if (object->type == OBJECT_ITEMSTACK)
		set_itemstack(r, object);
	else if (object->type == OBJECT_SPRITE)
	{
		if (!set_sprite(r, object))
			return (FALSE);
	}
	else if (object->type == OBJECT_ENTITY)
		set_entity(r, object, editor);
	else if (object->type == OBJECT_MODEL)
		set_model(r, object);
	else if (object->type == OBJECT_TRANSPO)
		set_transpo(r, object, editor);
	else if (object->type == OBJECT_LIGHT)
		set_light(r, object, editor);
	return (TRUE);
}

t_bool		create_object_renderable(t_editor *editor, int object_index,
	t_renderable *r)
{
	t_object	*object;

	object = &editor->objects->values[object_index];
	if (!set_objects(object, r, editor))
		return (FALSE);
	if (r->has_hitbox)
		r->show_hitbox = TRUE;
	if (object->scale.x == 0 && object->scale.y == 0 && object->scale.z == 0)
		object->scale = r->scale;
	else
		r->scale = object->scale;
	if (object->rotation.x == 0 && object->rotation.y == 0
		&& object->rotation.z == 0)
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

void		default_renderables(t_doom *doom)
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
