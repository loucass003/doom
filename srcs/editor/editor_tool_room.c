/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_room.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:09:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/17 15:37:52 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include "ressource.h"

void		init_rooms(t_editor *editor)
{
	append_rooms_array(&editor->rooms, (t_room){
		.walls = create_walls_array(15),
		.floor_texture = get_default_texture(&editor->doom->res_manager,
			TRUE),
		.ceiling_texture = get_default_texture(&editor->doom->res_manager,
			TRUE),
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

t_bool		select(t_room *curr_room, t_editor *editor, SDL_Event *event)
{
	t_vec2	vert0;

	vert0 = editor->points->vertices[curr_room->walls->values[0].indice];
	if (event->button.button == SDL_BUTTON_RIGHT
		|| (vert0.x == editor->grid_cell.x
		&& vert0.y == editor->grid_cell.y))
	{
		curr_room->closed = TRUE;
		select_room(editor, -1);
		editor->grid_cell = (t_vec2){ -1, -1 };
		editor->grid_cell_grab = GG_NONE;
		editor->line_start_cell = (t_vec2){ -1, -1 };
		update_rooms_gaps(editor);
		return (TRUE);
	}
	return (FALSE);
}

void		update_rooms(t_editor *editor, t_room *curr_room, int index)
{
	if (editor->grid_cell_grab == GG_NONE || editor->grid_cell_grab == GG_LINE)
	{
		append_2dvertices_array(&editor->points, editor->grid_cell);
		index = editor->points->len - 1;
		if (editor->grid_cell_grab == GG_NONE)
			gg_none(editor, curr_room, index);
		else if (editor->grid_cell_grab == GG_LINE)
			if (gg_line(editor, curr_room, index))
				return ;
		update_rooms_gaps(editor);
	}
	else if (editor->grid_cell_grab == GG_POINT && index != -1)
		gg_point(editor, curr_room, index);
}

void		editor_tool_room(t_editor *editor, SDL_Event *event)
{
	int		index;
	t_room	*curr_room;

	if (editor->line_start_cell.x == editor->grid_cell.x
		&& editor->line_start_cell.y == editor->grid_cell.y)
		return ;
	if (editor->current_room == -1)
		init_rooms(editor);
	curr_room = &editor->rooms->values[editor->current_room];
	if (curr_room->closed)
		return ;
	if (curr_room->walls->len >= 3)
		if (select(curr_room, editor, event))
			return ;
	if (event->button.button == SDL_BUTTON_RIGHT)
		return ;
	index = vertices2d_indexof(editor->points, editor->grid_cell);
	update_rooms(editor, curr_room, index);
}
