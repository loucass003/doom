/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_wall.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:44:38 by louali            #+#    #+#             */
/*   Updated: 2020/04/15 01:26:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"

void		append_array(t_renderable *r, t_vec3 face_normal,
	t_wall_section *ws)
{
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 0, 0 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 1, 0 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 1, 1 }));
	append_3dvertices_array(&r->normals, face_normal);
	append_2dvertices_array(&r->vertex, uv_setting(ws, (t_vec2){ 0, 1 }));
}

void		init_face(t_renderable *r, t_face_params p)
{
	t_face	face;

	ft_bzero(&face, sizeof(t_face));
	update_face(&face, p.ws, p.face_normal);
	face.normals_set = TRUE;
	face.normals_index[0] = p.n_start + p.indexes[0] + 1;
	face.normals_index[1] = p.n_start + p.indexes[1] + 1;
	face.normals_index[2] = p.n_start + p.indexes[2] + 1;
	face.vertex_set = TRUE;
	face.vertex_index[0] = p.v_start + p.indexes[0] + 1;
	face.vertex_index[1] = p.v_start + p.indexes[1] + 1;
	face.vertex_index[2] = p.v_start + p.indexes[2] + 1;
	face.vertices_index[0] = p.ws->vertices_index[p.indexes[0]] + 1;
	face.vertices_index[1] = p.ws->vertices_index[p.indexes[1]] + 1;
	face.vertices_index[2] = p.ws->vertices_index[p.indexes[2]] + 1;
	face.mtl_index = p.ws->material_index;
	face.wall_index = p.wall_index;
	face.wall_section = p.wall_section;
	face.room_index = p.room_index;
	append_faces_array(&r->faces, face);
}

t_vec3		get_face_normal(t_renderable *r, t_wall_section *ws)
{
	t_vec3	face_normal;
	t_vec4	p0;
	t_vec4	p1;
	t_vec4	p2;

	p0 = r->vertices->vertices[ws->vertices_index[0]];
	p1 = r->vertices->vertices[ws->vertices_index[1]];
	p2 = r->vertices->vertices[ws->vertices_index[2]];
	face_normal = get_triangle_normal(vec4_to_3(p0), vec4_to_3(p1),
		vec4_to_3(p2));
	if (ws->normal_type == 0)
		face_normal = ft_vec3_inv(face_normal);
	return (face_normal);
}

t_bool		create_wall(t_renderable *r, t_editor *editor, int indexes[3])
{
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;
	t_vec3			face_normal;
	t_face_params	params;

	room = &editor->rooms->values[indexes[0]];
	wall = &room->walls->values[indexes[1]];
	ws = &wall->wall_sections->values[indexes[2]];
	ws->material_index = r->materials->len;
	if (!append_mtllist(&r->materials, (t_mtl){
		.texture_map_set = TRUE, .texture_map = ws->texture->data.texture,
		.material_color_set = TRUE, .material_color = 0xFFFF0000 }))
		return (FALSE);
	face_normal = get_face_normal(r, ws);
	params = (t_face_params) { ws, r->normals->len, r->vertex->len, face_normal,
		indexes[0], indexes[1], indexes[2], {0, 0, 0} };
	append_array(r, face_normal, ws);
	ft_memcpy(&params.indexes, (int[3]){ 0, 1, 2 }, sizeof(int) * 3);
	init_face(r, params);
	ft_memcpy(&params.indexes, (int[3]){ 0, 2, 3 }, sizeof(int) * 3);
	init_face(r, params);
	return (TRUE);
}
