/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_wall.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:44:38 by louali            #+#    #+#             */
/*   Updated: 2020/02/20 17:18:30 by louali           ###   ########.fr       */
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

t_bool		update_wall(t_editor *editor, int room_index, int wall_index,
	int wall_section)
{
	t_face			*f1;
	t_face			*f2;
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;
	t_vec3			face_normal;
	int				start;

	if (editor->doom->main_context.type != CTX_EDITOR)
		return (TRUE);
	room = &editor->rooms->values[room_index];
	wall = &room->walls->values[wall_index];
	ws = &wall->wall_sections->values[wall_section];
	f1 = &get_map(editor)->faces->values[wall->faces_start
		+ (wall_section * 2)];
	f2 = &get_map(editor)->faces->values[wall->faces_start
		+ (wall_section * 2) + 1];
	face_normal = get_triangle_normal(
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[0] - 1]),
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[1] - 1]),
		vec4_to_3(get_map(editor)->pp_vertices[f1->vertices_index[2] - 1]));
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
	start = f1->vertex_index[0] - 1;
	get_map(editor)->vertex->vertices[start] = uv_setting(ws,
		(t_vec2){ 0, 0 });
	get_map(editor)->vertex->vertices[start + 1] = uv_setting(ws,
		(t_vec2){ 1, 0 });
	get_map(editor)->vertex->vertices[start + 2] = uv_setting(ws,
		(t_vec2){ 1, 1 });
	get_map(editor)->vertex->vertices[start + 3] = uv_setting(ws,
		(t_vec2){ 0, 1 });
	get_map(editor)->materials->values[
		ws->material_index].texture_map = ws->texture->data.texture;
	return (TRUE);
}

t_bool		create_wall(t_renderable *r, t_editor *editor, int room_index,
	int wall_index, int wall_section)
{
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;
	t_vec4			p0;
	t_vec4			p1;
	t_vec4			p2;
	t_vec3			face_normal;
	int				n_start;
	int				v_start;
	t_face			face;

	room = &editor->rooms->values[room_index];
	wall = &room->walls->values[wall_index];
	ws = &wall->wall_sections->values[wall_section];
	ws->material_index = r->materials->len;
	if (!append_mtllist(&r->materials, (t_mtl){
		.texture_map_set = TRUE, .texture_map = ws->texture->data.texture,
			.material_color_set = TRUE, .material_color = 0xFFFF0000 }))
		return (FALSE);
	p0 = r->vertices->vertices[ws->vertices_index[0]];
	p1 = r->vertices->vertices[ws->vertices_index[1]];
	p2 = r->vertices->vertices[ws->vertices_index[2]];
	face_normal = get_triangle_normal(vec4_to_3(p0), vec4_to_3(p1),
		vec4_to_3(p2));
	if (ws->normal_type == 0)
		face_normal = ft_vec3_inv(face_normal);
	n_start = r->normals->len;
	v_start = r->vertex->len;
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 0, 0 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 1, 0 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 1, 1 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 0, 1 }));
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
	return (TRUE);
}
