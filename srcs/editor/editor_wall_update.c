/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_wall_update.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 01:23:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 01:24:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"
#include "doom.h"

void		update_face(t_face *face, t_wall_section *ws, t_vec3 face_normal)
{
	face->hidden = ws->invisible;
	face->has_collision = ws->collisions;
	face->normal_type = ws->normal_type;
	face->double_sided = ws->normal_type == 2;
	face->face_normal = face_normal;
}

void		update_wall_uvs(t_editor *editor, t_face *f1, t_wall_section *ws)
{
	int		start;

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
}

t_bool		update_wall(t_editor *editor, int room_index, int wall_index,
	int wall_section)
{
	t_face			*f1;
	t_face			*f2;
	t_wall			*wall;
	t_wall_section	*ws;
	t_vec3			face_normal;

	if (editor->doom->main_context.type != CTX_EDITOR)
		return (TRUE);
	wall = &editor->rooms->values[room_index].walls->values[wall_index];
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
	update_face(f1, ws, face_normal);
	update_face(f2, ws, face_normal);
	update_wall_uvs(editor, f1, ws);
	return (TRUE);
}
