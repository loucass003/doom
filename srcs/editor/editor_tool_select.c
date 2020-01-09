/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_select.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:13:35 by llelievr          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2020/01/09 03:00:51 by llelievr         ###   ########.fr       */
=======
/*   Updated: 2020/01/09 01:54:28 by llelievr         ###   ########.fr       */
>>>>>>> 6f65c90baa069eeb84a17671f4ba4b1449421e66
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bool			seg_in_room(t_room *room, t_vec2 seg)
{
	int		i;
	int		wall0;
	int		wall1;

	i = -1;
	while (++i < room->walls->len)
	{
		wall0 = room->walls->values[i].indice;
		wall1 = room->walls->values[(i + 1) % room->walls->len].indice;
<<<<<<< HEAD
=======

>>>>>>> 6f65c90baa069eeb84a17671f4ba4b1449421e66
		if (seg.x == wall0 && seg.y == wall1)
			return (TRUE);
	}
	return (FALSE);
}

void			editor_tool_select(t_editor *editor, SDL_Event *event)
{
	const Uint8	*state = SDL_GetKeyboardState(NULL);
	int			index;
	t_vec2		seg;

	if (editor->grid_cell_grab != GG_OUTSIDE)
	{
		if (editor->current_room == -1)
		{
			editor->current_seg = (t_vec2){ -1, -1 };
			select_room(editor, get_close_room(editor));
		}
		else if (editor->current_room != -1 && editor->grid_cell_grab == GG_LINE)
		{
			editor->wall_section = 0;
			seg = get_close_seg(editor, &editor->rooms->values[editor->current_room], editor->grid_cell);
			editor->current_seg = seg;
			if (seg_in_room(&editor->rooms->values[editor->current_room], seg))
				select_room(editor, editor->current_room);
			else
				select_room(editor, get_close_room(editor));
		}
		else
		{
			editor->current_seg = (t_vec2){ -1, -1 };
			select_room(editor, -1);
		}
		editor_settings_update(editor);
	}
	if (editor->grid_cell_grab == GG_POINT)
	{
		index = vertices2d_indexof(editor->points, editor->grid_cell);
		editor->current_object = -1;
		if (index != -1)
			editor->current_point = index;
	}
	if (editor->grid_cell_grab == GG_OBJECT || (editor->current_object != -1 && (editor->grid_cell_grab == GG_NONE || editor->grid_cell_grab == GG_OUTSIDE)))
		editor_tool_objects(editor, event);
	if (editor->grid_cell_grab == GG_PLAYER)
		editor_tool_player(editor, event);
}
