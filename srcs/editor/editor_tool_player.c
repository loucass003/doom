/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_player.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 13:21:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/03 15:24:24 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "editor.h"

void			editor_tool_player_move(t_editor *editor)
{
	if (editor->player_grab)
	{
        editor->player_pos.x = editor->grid_cell.x;
        editor->player_pos.z = editor->grid_cell.y;
	}
}

void			editor_tool_player_release(t_editor *editor)
{
	if (editor->player_grab)
		editor->player_grab = FALSE; 
}

void			editor_tool_player(t_editor *editor, SDL_Event *event)
{
    int         room;
    float       y;

	if ((!editor->player_set || editor->grid_cell_grab == GG_NONE) && editor->grid_cell_grab != GG_OUTSIDE)
	{
        int room = point_in_rooms(editor, editor->grid_cell);
        y = 0;
        if (room != -1)
            y = room_height_range(editor, &editor->rooms->values[room]).x;
		editor->player_pos = (t_vec3){editor->grid_cell.x, y, editor->grid_cell.y};
        editor->player_set = TRUE;
        editor_settings_update(editor);
	}
	if (editor->player_set && editor->grid_cell_grab == GG_PLAYER)
    {
        editor->player_grab = TRUE;
    }
    // if (editor->player_set && (editor->grid_cell_grab == GG_NONE || editor->grid_cell_grab == GG_OUTSIDE))
    // {
    //     editor->player_grab = FALSE;
    // }
}