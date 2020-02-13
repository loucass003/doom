/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_create_walls.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 11:36:27 by louali            #+#    #+#             */
/*   Updated: 2020/02/13 14:23:53 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"
#include "doom.h"

t_bool	add_room_points(t_editor *editor, t_room *room, t_renderable *r)
{
	int			j;
	t_wall		wall;
	t_vec3		point;

	j = -1;
	while (++j < room->walls->len)
	{
		wall = room->walls->values[j];
		point = editor_to_world((t_vec3){
			editor->points->vertices[wall.indice].x, 0,
			editor->points->vertices[wall.indice].y });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x,
			wall.floor_height, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, 1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x,
			wall.ceiling_height, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, -1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
	}
	return (TRUE);
}

void	floor_ceil(t_renderable *r, t_room *room, int i)
{
	int		j;
	int		*filter;

	j = -1;
	filter = malloc(room->walls->len * sizeof(int));
	while (++j < room->walls->len)
		filter[j] = room->room_vertices_start + j * 2;
	triangulate_floor_ceil(r, (t_vec3){ 0, -1, 0 }, filter, room->walls->len,
		room->floor_normal, (i * 2), i, room->floor_uv_offset,
		room->floor_uv_repeat);
	room->ceilling_start = r->faces->len;
	j = -1;
	while (++j < room->walls->len)
		filter[j] = room->room_vertices_start + (j * 2) + 1;
	triangulate_floor_ceil(r, (t_vec3){ 0, 1, 0 }, filter, room->walls->len,
		room->ceil_normal, (i * 2) + 1, i, room->ceil_uv_offset,
		room->ceil_uv_repeat);
	room->ceilling_end = r->faces->len;
	free(filter);
}

t_bool	create_map_points_and_floor(t_editor *editor, t_renderable *r)
{
	int		i;
	t_room	*room;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (!room->closed)
			continue;
		if (!append_mtllist(&r->materials, (t_mtl){.texture_map_set = TRUE,
			.texture_map = room->floor_texture->data.texture,
			.material_color_set = TRUE, .material_color = 0xFFFF0000 }))
			return (free_renderable(r, FALSE, FALSE));
		if (!append_mtllist(&r->materials, (t_mtl){.texture_map_set = TRUE,
			.texture_map = room->ceiling_texture->data.texture,
			.material_color_set = TRUE, .material_color = 0xFFFF0000 }))
			return (free_renderable(r, FALSE, FALSE));
		room->room_vertices_start = r->vertices->len;
		add_room_points(editor, room, r);
		room->floor_start = r->faces->len;
		floor_ceil(r, room, i);
	}
	editor->walls_faces_start = r->faces->len;
	return (TRUE);
}

void	creat_walls(t_renderable *r, t_editor *editor, int i, t_room *room)
{
	int		k;
	int		j;
	t_wall	*w0;

	j = -1;
	while (++j < room->walls->len)
	{
		w0 = &room->walls->values[j];
		w0->materials_start = r->materials->len;
		w0->faces_start = r->faces->len;
		k = -1;
		while (++k < w0->wall_sections->len)
			create_wall(r, editor, i, j, k);
	}
}

t_bool	create_walls(t_editor *editor, t_renderable *r)
{
	int			i;
	t_room		*room;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (!room->closed)
			continue;
		room->walls_start = r->faces->len;
		floor_visibility(editor, r, i);
		get_room_gaps(editor, room);
		creat_walls(r, editor, i, room);
	}
	return (TRUE);
}
