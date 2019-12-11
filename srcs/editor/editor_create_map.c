/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_create_map.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 13:41:47 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/11 01:01:30 by llelievr         ###   ########.fr       */
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
		triangulate_floor_ceil(r, (t_vec3){ 0, -1, 0 }, filter, room->walls->len, 0);
		room->ceilling_start = r->faces->len;
		j = -1;
		while (++j < room->walls->len)
			filter[j] = room->room_vertices_start + (j * 2) + 1;
		triangulate_floor_ceil(r, (t_vec3){ 0, 1, 0 }, filter, room->walls->len, 1);
		free(filter);
	}
	return (TRUE);
}

t_bool	create_walls(t_editor *editor, t_renderable *r)
{
	int		i;
	int		j;

	i = -1;
	while (++i < editor->rooms->len)
	{
		t_room *room = &editor->rooms->values[i];
		if (!room->closed)
			continue;
		get_room_gaps(editor, room);
		j = -1;
		while (++j < room->walls->len)
		{
			t_wall w0 = room->walls->values[j];
			int next = ((j + 1) % room->walls->len);
			t_wall w1 = room->walls->values[next];

			/*
				t_vec4 p0 = r->vertices->vertices[i * 2];
				t_vec4 p1 = r->vertices->vertices[next * 2];
				t_vec4 p2 = r->vertices->vertices[next * 2 + 1];
				t_vec4 p3 = r->vertices->vertices[i * 2 + 1];
			*/
			
		
			if (w0.start_rooms_range && w1.end_rooms_range && w0.start_rooms_range->len != w1.end_rooms_range->len)
			{
				printf("PROBLEM !\n");
				continue; 
			}
			if (w0.start_rooms_range && w1.end_rooms_range)
			{
				printf("CALL WALL\n");
				float start = w0.floor_height;
				float startb = w1.floor_height;
				int lr_0 = 0;
				int lr_1 = 0;
				t_vec4	r0;
				t_vec4	r1;
				for (int k = 0; k < w0.start_rooms_range->len; k++)
				{
					r0 = w0.start_rooms_range->vertices[k];
					r1 = w1.end_rooms_range->vertices[k];
					int vertices_index[4] = (int [4]){ room->room_vertices_start + j * 2, room->room_vertices_start + next * 2, room->room_vertices_start + next * 2 + 1, room->room_vertices_start + (j * 2 + 1) };
					if (start < r0.x && startb < r1.x)
					{
						//if (start != w0.floor_height)
						{
							vertices_index[3] = editor->rooms->values[(int)r0.z].room_vertices_start + ((int)r0.w * 2);
						}

						//if (startb != w1.floor_height)
						{
							vertices_index[2] = editor->rooms->values[(int)r1.z].room_vertices_start + ((int)r1.w * 2);
						}

						if (start != w0.floor_height)
						{
							vertices_index[0] = editor->rooms->values[(int)r0.z].room_vertices_start + (int)r0.w * 2 + 1;
						}

						if (startb != w1.floor_height)
						{
							vertices_index[1] = editor->rooms->values[(int)r1.z].room_vertices_start + (int)r1.w * 2 + 1;
						}
						printf("gap (%f %f - %f) (%f %f - %f)\n", start, r0.x, r0.z, startb, r1.x, r1.z);
						create_wall(r, room, j, vertices_index);
						start = r0.y;
						startb = r1.y;
						lr_0 = k;
						lr_1 = k;
					}
					// else if (r0.x < w0.ceiling_height && r1.x < w1.ceiling_height)
					// {
					// 	printf("ENTER\n");
					// 	start = r0.x;
					// 	startb = r1.x;
					// }
				}
				if (start < w0.ceiling_height && start != w0.floor_height && startb < w1.ceiling_height && startb != w1.floor_height)
				{
					int vertices_index[4] = (int [4]){ room->room_vertices_start + j * 2, room->room_vertices_start + next * 2, room->room_vertices_start + next * 2 + 1, room->room_vertices_start + (j * 2 + 1) };
					
					vertices_index[0] = editor->rooms->values[(int)r0.z].room_vertices_start + ((int)r0.w * 2) + 1;
					vertices_index[1] = editor->rooms->values[(int)r1.z].room_vertices_start + ((int)r1.w * 2) + 1;
					
					create_wall(r, room, j, vertices_index);
					printf("gap LAST (%f %f) (%f %f)\n", start, w0.ceiling_height, startb, w1.ceiling_height);
				}
			}
			else
			{
				int vertices_index[4] = (int [4]){ room->room_vertices_start + j * 2, room->room_vertices_start + next * 2, room->room_vertices_start + next * 2 + 1, room->room_vertices_start + (j * 2 + 1) };
				create_wall(r, room, j, vertices_index);
			}
		}
	}
	return (TRUE);
}

t_bool	create_map(t_editor *editor)
{
	t_renderable	r;

	if (!create_renderable(&r, RENDERABLE_ROOM))  
		return (FALSE);
	if(!(r.materials = create_mtllist(editor->rooms->len * 20)))
		return (free_renderable(&r, FALSE));
	create_map_points_and_floor(editor, &r);
	create_walls(editor, &r);
	post_process_renderable(editor->doom, &r, TRUE);
	r.scale = (t_vec3){ 1, 1, 1 };
	r.dirty = TRUE; 
	r.visible = TRUE;
	//r->wireframe = TRUE;
	r.wireframe_color = 0xFFFF0000;
	if (!append_renderables_array(&editor->doom->renderables, r))  
		return (FALSE);
	return (TRUE);
}
