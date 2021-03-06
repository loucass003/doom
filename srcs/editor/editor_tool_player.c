/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_player.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 13:21:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/17 15:03:03 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "editor.h"
#include "doom.h"

void			editor_tool_player_move(t_editor *editor)
{
	if (editor->player_grab)
	{
		editor->doom->player.spawn_data.position.x = editor->grid_cell.x;
		editor->doom->player.spawn_data.position.z = editor->grid_cell.y;
	}
}

void			editor_tool_player_release(t_editor *editor)
{
	if (editor->player_grab)
		editor->player_grab = FALSE;
}

void			editor_tool_player(t_editor *editor)
{
	float		y;
	int			room;

	if ((!editor->player_set || editor->grid_cell_grab == GG_NONE)
		&& editor->grid_cell_grab != GG_OUTSIDE)
	{
		room = point_in_rooms(editor, editor->grid_cell);
		y = 0;
		if (room != -1 && !editor->player_set)
			y = room_height_range(&editor->rooms->values[room]).x;
		editor->doom->player.spawn_data.position = (t_vec3){editor->grid_cell.x,
			y, editor->grid_cell.y};
		editor->player_set = TRUE;
		editor_settings_update(editor);
	}
	if (editor->player_set && editor->grid_cell_grab == GG_PLAYER)
	{
		editor->player_grab = TRUE;
	}
}
