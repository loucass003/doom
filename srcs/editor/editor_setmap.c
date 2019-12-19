/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/19 18:57:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"

t_bool	triangulate_floor_ceil(t_renderable *r, t_vec3 n, int *filter, int filter_len, int mtl, int room_index)
{
	t_mat4			p_inv;
	t_mat4			reverse;
	int				i;

	if (!compute_change_of_basis(n, &p_inv, &reverse))
		return (FALSE);
	i = -1;
	while (++i < filter_len)
		r->vertices->vertices[filter[i]] = mat4_mulv4(p_inv,
			r->vertices->vertices[filter[i]]);
	if (!ear_clip2(filter, filter_len, r->vertices, &r->faces, 0, mtl, room_index))
		return (FALSE);
	uv_mapping(r->vertices, r->vertex, filter, filter_len);
	i = -1;
	while (++i < filter_len)
		r->vertices->vertices[filter[i]] = mat4_mulv4(reverse,
			r->vertices->vertices[filter[i]]);
	return (TRUE);
}

t_vec3		editor_to_world(t_vec3 pos)
{
	float	ratio = 1. / 5.;

	pos.x *= ratio;
	pos.z *= ratio;
	return (pos);
}

t_bool		update_wall(t_editor *editor, int room_index, int wall_index, int wall_section)
{
	// t_renderable	*r;
	t_face			*f1;
	t_face			*f2;
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;

	if (editor->doom->main_context.type != CTX_EDITOR)
		return (TRUE);

	// if (!(r = room->r))
	// 	return (FALSE);
	room = &editor->rooms->values[room_index];
	wall = &room->walls->values[wall_index];
	ws = &wall->wall_sections->values[wall_section];
	f1 = &get_map(editor)->faces->values[wall->faces_start + (wall_section * 2)];
	f2 = &get_map(editor)->faces->values[wall->faces_start + (wall_section * 2) + 1];

	t_vec3 face_normal = get_triangle_normal(
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[0] - 1]),
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[1] - 1]),
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[2] - 1])
	);

	if (ws->normal_type == 0)
		face_normal = ft_vec3_inv(face_normal);

	f1->hidden = ws->invisible;
	f1->has_collision = ws->collisions;
	f1->face_normal = face_normal;
	f1->normal_type = ws->normal_type;
	f1->double_sided = ws->normal_type == 2;
	

	f2->hidden = ws->invisible;
	f2->has_collision = ws->collisions;
	f2->face_normal = face_normal;
	f2->normal_type = ws->normal_type;
	f2->double_sided = ws->normal_type == 2;

	get_map(editor)->materials->values[ws->material_index].texture_map = ws->texture->data.texture;
	return (TRUE);
}

t_bool		update_floor(t_editor *editor, int room_index, t_bool floor)
{
	t_room	*room;

	if (editor->doom->main_context.type != CTX_EDITOR)
		return (TRUE);

	room = &editor->rooms->values[room_index];
	if (floor)
		get_map(editor)->materials->values[room_index * 2].texture_map = room->floor_texture->data.texture;
	else
	 	get_map(editor)->materials->values[room_index * 2 + 1].texture_map = room->ceiling_texture->data.texture;
	return (TRUE);
}

t_bool		create_wall(t_renderable *r, t_editor *editor, int room_index, int wall_index, int wall_section)
{
	t_room 	*room = &editor->rooms->values[room_index];
	t_wall	*wall = &room->walls->values[wall_index];
	t_wall_section	*ws = &wall->wall_sections->values[wall_section];
	
	// if (!wall->collisions && wall->invisible)
	// 	continue;
	// int	next = (i + 1) % room->walls->len;
//	printf("MATERIAL %d\n", r->materials->len);
	ws->material_index = r->materials->len;
	if (!append_mtllist(&r->materials, (t_mtl){ 
		.texture_map_set = TRUE, .texture_map = ws->texture->data.texture, .material_color_set = TRUE, .material_color = 0xFFFF0000 }))
	return (FALSE);

	t_vec4 p0 = r->vertices->vertices[ws->vertices_index[0]];
	t_vec4 p1 = r->vertices->vertices[ws->vertices_index[1]];
	t_vec4 p2 = r->vertices->vertices[ws->vertices_index[2]];
	t_vec3 face_normal = get_triangle_normal(vec4_to_3(p0), vec4_to_3(p1), vec4_to_3(p2));

	if (ws->normal_type == 0)
		face_normal = ft_vec3_inv(face_normal);
	
	int	n_start = r->normals->len;
	int	v_start = r->vertex->len;
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });

	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, (t_vec2){ 1, 0 });

	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, (t_vec2){ 1, 1 });

	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, (t_vec2){ 0, 1 });
	t_face face;
	
	ft_bzero(&face, sizeof(t_face));
	face.hidden = ws->invisible;
	face.has_collision = ws->collisions;
	face.face_normal = face_normal;
	face.normal_type = ws->normal_type;
	face.double_sided = ws->normal_type == 2;
	face.normals_set = TRUE;
	face.normals_index[0] = n_start + 1;
	face.normals_index[1] = n_start + 2 + 1;
	face.normals_index[2] = n_start + 3 + 1;
	face.vertex_set = TRUE;
	face.vertex_index[0] = v_start + 1;
	face.vertex_index[1] = v_start + 2 + 1;
	face.vertex_index[2] = v_start + 3 + 1;
	face.vertices_index[0] = ws->vertices_index[0] + 1;
	face.vertices_index[1] = ws->vertices_index[2] + 1;
	face.vertices_index[2] = ws->vertices_index[3] + 1;
	face.mtl_index = ws->material_index;
	face.wall_index = wall_index;
	face.wall_section = wall_section;
	face.room_index = room_index;
	append_faces_array(&r->faces, face);
	
	ft_bzero(&face, sizeof(t_face));
	face.hidden = ws->invisible;
	face.has_collision = ws->collisions;
	face.normal_type = ws->normal_type;
	face.double_sided = ws->normal_type == 2;
	face.normals_set = TRUE;
	face.normals_index[0] = n_start + 1;
	face.normals_index[1] = n_start + 1 + 1;
	face.normals_index[2] = n_start + 2 + 1;
	face.vertex_set = TRUE;
	face.vertex_index[0] = v_start + 1;
	face.vertex_index[1] = v_start + 1 + 1;
	face.vertex_index[2] = v_start + 2 + 1;
	face.vertices_index[0] = ws->vertices_index[0] + 1;
	face.vertices_index[1] = ws->vertices_index[1] + 1;
	face.vertices_index[2] = ws->vertices_index[2] + 1;
	face.mtl_index = ws->material_index;
	face.wall_index = wall_index;
	face.wall_section = wall_section;
	face.room_index = room_index;
	append_faces_array(&r->faces, face);
}

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
		// ft_memcpy(r, object->of.model->data.model, sizeof(t_renderable));
		copy_renderable(object->of.model->data.model, r);
		r->scale = (t_vec3){ 1, 1, 1 };
		r->dirty = TRUE;
		r->position = editor_to_world(object->pos);
		r->position.y += r->scale.y;
	}
	if (r->has_hitbox)
		r->show_hitbox = TRUE;
	r->object_index = object_index;
	r->no_light = object->no_light;
	object->r = r;
	return (TRUE);
}

t_bool		editor_setmap(t_editor *editor) 
{
	int		i;

	editor->doom->main_context.type = CTX_EDITOR;
	editor->doom->renderables->len = 0;
	editor->doom->skybox_index = -1;
	editor->map_renderable = editor->doom->renderables->len;
	t_renderable r;
	create_map(&r, editor);
	if (!append_renderables_array(&editor->doom->renderables, r))
		return (FALSE);
	// i = -1;
	// while (++i < editor->rooms->len)
	// {
	// 	t_room			*room = &editor->rooms->values[i];
	// 	t_renderable	r;

	// 	if (!room->closed)
	// 		continue;
	// 	if (!create_room_renderable(&r, editor, room))
	// 		return (FALSE);
	// 	if (!append_renderables_array(&editor->doom->renderables, r))
	// 		return (FALSE);
	// 	room->r = &editor->doom->renderables->values[editor->doom->renderables->len - 1];
	// }
	i = -1;
	while (++i < editor->objects->len)
	{
		t_object		*object = &editor->objects->values[i];
		t_renderable	r;

		if (object->type == OBJECT_NONE)
			continue ;
		if (!create_object_renderable(editor, i, &r))
			return (FALSE);
		if (!append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
		object->r = &editor->doom->renderables->values[editor->doom->renderables->len - 1];
		if (object->r->has_hitbox)
			object->r->show_hitbox = FALSE;
	}
	spawn_player(editor->doom);
	return (TRUE);
}
