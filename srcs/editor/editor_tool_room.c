/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_room.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:09:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/10 02:11:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include "ressource.h"

void		editor_tool_room(t_editor *editor, SDL_Event *event)
{
	if (editor->line_start_cell.x == editor->grid_cell.x 
		&& editor->line_start_cell.y == editor->grid_cell.y)
		return ;

	if (editor->current_room == -1)
	{
		printf("NEW_ROOM\n");
		append_rooms_array(&editor->rooms, (t_room){
			.walls = create_walls_array(15), 
			.floor_texture = get_default_texture(&editor->doom->res_manager, TRUE), 
			.ceiling_texture = get_default_texture(&editor->doom->res_manager, TRUE),
			.floor_invisible = FALSE,
			.ceil_invisible = FALSE,
			.ambiant_light = AMBIANT_LIGHT,
			.floor_collision = TRUE,
			.ceil_collision = TRUE,
			.floor_normal = 1,
			.ceil_normal = 1,
			.floor_uv_repeat = (t_vec2){ 1, 1 },
			.ceil_uv_repeat = (t_vec2){ 1, 1 }
		});
		select_room(editor, editor->rooms->len - 1);
	}

	t_room	*curr_room = &editor->rooms->values[editor->current_room];
	if (curr_room->closed)
		return ;
	if (curr_room->walls->len >= 3)
	{
		t_vec2 vert0 = editor->points->vertices[curr_room->walls->values[0].indice];
		if (event->button.button == SDL_BUTTON_RIGHT || (vert0.x == editor->grid_cell.x && vert0.y == editor->grid_cell.y))
		{
			curr_room->closed = TRUE;
			select_room(editor, -1);
			editor->grid_cell = (t_vec2){ -1, -1 };
			editor->grid_cell_grab = GG_NONE;
			editor->line_start_cell = (t_vec2){ -1, -1 };
			printf("CLOSE %d\n", curr_room->walls->len);
			update_rooms_gaps(editor);
			return ;
		}
	}

	if (event->button.button == SDL_BUTTON_RIGHT)
		return ;
	int	index = vertices2d_indexof(editor->points, editor->grid_cell);
	
	if (editor->grid_cell_grab == GG_NONE || editor->grid_cell_grab == GG_LINE)
	{
		// printf("NEW POINT\n");
		append_2dvertices_array(&editor->points, editor->grid_cell);
		index = editor->points->len - 1;
		if (editor->grid_cell_grab == GG_NONE)
		{
			append_walls_array(&curr_room->walls, init_wall(index));
			if (room_intersect(editor, curr_room, curr_room, TRUE))
			{
				splice_2dvertices_array(editor->points, index, 1);
				splice_walls_array(curr_room->walls, curr_room->walls->len - 1, 1);
			}
			else
				editor->line_start_cell = editor->grid_cell;
		}
		else if (editor->grid_cell_grab == GG_LINE)
		{
			append_walls_array(&curr_room->walls, init_wall(index));
			if (room_intersect(editor, curr_room, curr_room, TRUE))
			{
				splice_2dvertices_array(editor->points, index, 1);
				splice_walls_array(curr_room->walls, curr_room->walls->len - 1, 1);
				return ;
			}
			else
				editor->line_start_cell = editor->grid_cell;
			insert_point(editor, editor->close_seg, index);
		}
		update_rooms_gaps(editor);
	}
	else if (editor->grid_cell_grab == GG_POINT && index != -1)
	{
		append_walls_array(&curr_room->walls, init_wall(index));
		if (room_intersect(editor, curr_room, curr_room, TRUE))
		{
			splice_walls_array(curr_room->walls, curr_room->walls->len - 1, 1);
		}
		else
			editor->line_start_cell = editor->grid_cell;
		update_rooms_gaps(editor);
	}
}
