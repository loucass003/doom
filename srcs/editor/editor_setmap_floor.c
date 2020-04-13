/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap_floor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:41:09 by louali            #+#    #+#             */
/*   Updated: 2020/04/13 16:52:13 by Lisa             ###   ########.fr       */
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

t_bool		triangulate_floor_ceil(t_renderable *r, t_vec3 n, int *filter,
	int filter_len, int normal_type, int mtl, int room_index,
	t_vec2 offset, t_vec2 repeat)
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
	if (!ear_clip2(filter, filter_len, r->vertices, &r->faces,
		normal_type, mtl, room_index))
		return (FALSE);
	uv_mapping(r->vertices, r->vertex, filter, filter_len, offset, repeat);
	i = -1;
	while (++i < filter_len)
		r->vertices->vertices[filter[i]] = mat4_mulv4(reverse,
			r->vertices->vertices[filter[i]]);
	return (TRUE);
}

t_bool		floor_visibility(t_editor *editor, t_renderable *r, int room_index)
{
	int		i;
	t_room	*room;

	room = &editor->rooms->values[room_index];
	i = room->floor_start - 1;
	while (++i < room->ceilling_start)
	{
		r->faces->values[i].hidden = room->floor_invisible;
		r->faces->values[i].normal_type = room->floor_normal;
		r->faces->values[i].double_sided = room->floor_normal == 2;
		r->faces->values[i].has_collision = room->floor_collision;
	}
	i = room->ceilling_start - 1;
	while (++i < room->ceilling_end)
	{
		r->faces->values[i].hidden = room->ceil_invisible;
		r->faces->values[i].normal_type = room->ceil_normal;
		r->faces->values[i].double_sided = room->ceil_normal == 2;
		r->faces->values[i].has_collision = room->ceil_collision;
	}
	r->dirty = TRUE;
	compute_collidables(r);
	return (TRUE);
}

t_bool		update_floor(t_editor *editor, int room_index, t_bool floor)
{
	t_room	*room;

	if (editor->doom->main_context.type != CTX_EDITOR)
		return (TRUE);
	room = &editor->rooms->values[room_index];
	if (floor)
		get_map(editor)->materials->values[room_index
			* 2].texture_map = room->floor_texture->data.texture;
	else
		get_map(editor)->materials->values[room_index
			* 2 + 1].texture_map = room->ceiling_texture->data.texture;
	floor_visibility(editor, get_map(editor), room_index);
	return (TRUE);
}
