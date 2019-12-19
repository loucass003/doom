/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_create_map.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 13:41:47 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/19 02:08:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"
#include "doom.h"

t_bool	add_room_points(t_editor *editor, t_room *room, t_renderable *r)
{
	int		j;
	t_wall	wall;
	t_vec3	point;

	j = -1;
	while (++j < room->walls->len)
	{
		wall = room->walls->values[j];
		point = editor_to_world((t_vec3){ 
			editor->points->vertices[wall.indice].x, 0, 
			editor->points->vertices[wall.indice].y });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, wall.floor_height, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, 1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, wall.ceiling_height, point.z, 1 });
		append_3dvertices_array(&r->normals, (t_vec3){ 0, -1, 0 });
		append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 });
	}
	return (TRUE);
}

t_bool	create_map_points_and_floor(t_editor *editor, t_renderable *r)
{
	int		i;
	t_room	*room;

	printf("MATERIALS %d\n", r->materials->len);
	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (!room->closed)
			continue;
		if (!append_mtllist(&r->materials, (t_mtl){ 
				.texture_map_set = TRUE, .texture_map = room->floor_texture->data.texture, .material_color_set = TRUE, .material_color = 0xFFFF0000 }))
			return (free_renderable(&r, FALSE));
		if (!append_mtllist(&r->materials, (t_mtl){ 
				.texture_map_set = TRUE, .texture_map = room->ceiling_texture->data.texture, .material_color_set = TRUE, .material_color = 0xFFFF0000 }))
			return (free_renderable(&r, FALSE));
		
		room->room_vertices_start = r->vertices->len;
		add_room_points(editor, room, r);
		int	*filter = malloc(room->walls->len * sizeof(int));
		room->floor_start = r->faces->len;
		int j = -1;
		while (++j < room->walls->len)
			filter[j] = room->room_vertices_start + j * 2;
		triangulate_floor_ceil(r, (t_vec3){ 0, -1, 0 }, filter, room->walls->len, (i * 2), i);
		room->ceilling_start = r->faces->len;
		j = -1;
		while (++j < room->walls->len)
			filter[j] = room->room_vertices_start + (j * 2) + 1;
		triangulate_floor_ceil(r, (t_vec3){ 0, 1, 0 }, filter, room->walls->len, (i * 2) + 1, i);
		free(filter);
	}
	editor->walls_faces_start = r->faces->len;
	return (TRUE);
}

t_bool	create_walls(t_editor *editor, t_renderable *r)
{
	int		i;
	int		j;
	int		k;
	t_room	*room;
	t_wall	*w0;

	// r->materials->len = editor->rooms->len * 2;
	// r->faces->len = editor->walls_faces_start;
	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (!room->closed)
			continue;
		room->walls_start = r->faces->len;
		get_room_gaps(editor, room);
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
	return (TRUE);
}

t_bool	post_process_map(t_editor *editor, t_renderable *r, t_bool replace)
{
	post_process_renderable(editor->doom, r, TRUE, replace);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE; 
	r->visible = TRUE;
	r->wireframe_color = 0xFFFF0000;
}

t_bool	create_map(t_renderable	*r, t_editor *editor)
{

	if (!create_renderable(r, RENDERABLE_MAP))  
		return (FALSE);
	if(!(r->materials = create_mtllist(80)))
		return (FALSE);
	create_map_points_and_floor(editor, r);
	create_walls(editor, r);
	post_process_map(editor, r, FALSE);
	
	return (TRUE);
}

t_renderable	*get_map(t_editor *editor)
{
	if (editor->map_renderable == -1)
		return (NULL);
	return &editor->doom->renderables->values[editor->map_renderable];
}
