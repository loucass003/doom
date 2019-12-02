/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/02 18:15:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"

t_bool	triangulate_floor_ceil(t_renderable *r, t_vec3 n, int *filter, int filter_len, int mtl)
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
	ear_clip2(filter, filter_len, r->vertices, &r->faces, 0, mtl);
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

t_bool		create_room_mesh(t_renderable *r, t_editor *editor, t_room *room)
{
	int		i;
	t_wall	*wall;
	t_vec3	point;

	i = -1;
	while (++i < room->walls->len)
	{
		wall = &room->walls->values[i];
		t_vec2 v = editor->points->vertices[wall->indice];
		point = editor_to_world((t_vec3){ v.x, 0, v.y });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, 0, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, 1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, 10, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, -1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
	}

	int	*filter = malloc(room->walls->len * sizeof(int));
	i = -1;
	while (++i < room->walls->len)
		filter[i] = i * 2;
	triangulate_floor_ceil(r, (t_vec3){ 0, -1, 0 }, filter, room->walls->len, 0);
	i = -1;
	while (++i < room->walls->len)
		filter[i] = (i * 2) + 1;
	triangulate_floor_ceil(r, (t_vec3){ 0, 1, 0 }, filter, room->walls->len, 1);
	i = -1;
	while (++i < room->walls->len)
	{
		t_wall	*wall = &room->walls->values[i];
		if (!wall->collisions && wall->invisible)
			continue;
		int	next = (i + 1) % room->walls->len;
		t_vec4 p0 = r->vertices->vertices[i * 2];
		t_vec4 p1 = r->vertices->vertices[next * 2];
		t_vec4 p2 = r->vertices->vertices[next * 2 + 1];
		t_vec3 face_normal = get_triangle_normal(vec4_to_3(p0), vec4_to_3(p1), vec4_to_3(p2));

		if (wall->normal_type == 0)
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
		face.hidden = wall->invisible;
		face.has_collision = wall->collisions;
		face.face_normal = face_normal;
		face.normal_type = wall->normal_type;
		face.double_sided = wall->normal_type == 2;
		face.normals_set = TRUE;
		face.normals_index[0] = n_start + 1;
		face.normals_index[1] = n_start + 2 + 1;
		face.normals_index[2] = n_start + 3 + 1;
		face.vertex_set = TRUE;
		face.vertex_index[0] = v_start + 1;
		face.vertex_index[1] = v_start + 2 + 1;
		face.vertex_index[2] = v_start + 3 + 1;
		face.vertices_index[0] = i * 2 + 1;
		face.vertices_index[1] = next * 2 + 1 + 1;
		face.vertices_index[2] = i * 2 + 1 + 1;
		face.mtl_index = i + 2;
		face.wall_index = i;
		append_faces_array(&r->faces, face);
		
		ft_bzero(&face, sizeof(t_face));
		face.hidden = wall->invisible;
		face.has_collision = wall->collisions;
		face.normal_type = wall->normal_type;
		face.double_sided = wall->normal_type == 2;
		face.normals_set = TRUE;
		face.normals_index[0] = n_start + 1;
		face.normals_index[1] = n_start + 1 + 1;
		face.normals_index[2] = n_start + 2 + 1;
		face.vertex_set = TRUE;
		face.vertex_index[0] = v_start + 1;
		face.vertex_index[1] = v_start + 1 + 1;
		face.vertex_index[2] = v_start + 2 + 1;
		face.vertices_index[0] = i * 2 + 1;
		face.vertices_index[1] = next * 2 + 1;
		face.vertices_index[2] = next * 2 + 1 + 1;
		face.mtl_index = i + 2;
		face.wall_index = i;
		append_faces_array(&r->faces, face);
	}
	post_process_renderable(editor->doom, r, TRUE);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE; 
	r->visible = TRUE;
	//r->wireframe = TRUE;
	r->wireframe_color = 0xFFFF0000;
	return (TRUE);
}

t_bool		create_room_renderable(t_renderable *r, t_editor *editor, t_room *room)
{
	if (!create_renderable(r, RENDERABLE_ROOM))  
		return (FALSE);
	r->of.data.room = room;
	room->r = r;
	if(!(r->materials = create_mtllist(room->walls->len + 2)))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, (t_mtl){ 
			.texture_map_set = TRUE, .texture_map = room->floor_texture->data.texture }))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, (t_mtl){ 
			.texture_map_set = TRUE, .texture_map = room->ceiling_texture->data.texture }))
		return (free_renderable(&r, FALSE));
	int i = -1;
	while (++i < room->walls->len)
		if (!append_mtllist(&r->materials, (t_mtl){ 
			.texture_map_set = TRUE, .texture_map = room->walls->values[i].texture->data.texture }))
			return (free_renderable(&r, FALSE));
	create_room_mesh(r, editor, room);
	return (TRUE);
}

t_bool		create_object_renderable(t_editor *editor, int object_index, t_renderable *r)
{
	t_object	*object = &editor->objects->values[object_index];

	// if (object->type == OBJECT_PLAYER)
	// {
	// 	editor->player_pos = editor_to_world(object->pos);
	// 	editor->player_pos.y += editor->doom->player.entity.radius.y;
	// 	if (object->r)
	// 	{
	// 		splice_renderables_array(editor->doom->renderables, renderables_indexof(editor->doom->renderables, object->r), 1);
	// 		object->r = NULL;
	// 	}
	// }
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
		ft_memcpy(r, object->of.model->data.model, sizeof(t_renderable));
		r->scale = (t_vec3){ 1, 1, 1 };
		r->dirty = TRUE;
		r->position = editor_to_world(object->pos);
		r->position.y += r->scale.y;
	}
	r->object_index = object_index;
	r->no_light = object->no_light;

	return (TRUE);
}


t_bool		editor_setmap(t_editor *editor) 
{
	int		i;

	editor->doom->main_context.type = CTX_EDITOR;
	editor->doom->renderables->len = 0;
	editor->doom->skybox_index = -1;
	i = -1;
	while (++i < editor->rooms->len)
	{
		t_room			*room = &editor->rooms->values[i];
		t_renderable	r;

		if (!room->closed)
			continue;
		if (!create_room_renderable(&r, editor, room))
			return (FALSE);
		if (!append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
	}
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
	}
	spawn_player(editor->doom);
	return (TRUE);
}
