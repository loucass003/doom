/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_select.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:13:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/14 04:15:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static	void	swap_normal(t_editor *editor)
{
	t_room	*curr_room;
	int		index0;
	int		index1;

	printf("SWAP NORMAL\n");
	curr_room = &editor->rooms->values[editor->current_room];
	index0 = wall_indexof_by_indice(curr_room->walls, editor->close_seg.x);
	index1 = wall_indexof_by_indice(curr_room->walls, editor->close_seg.y);
	if (index0 > index1 && index1 != 0)
	{
		int tmp = index0;
		index0 = index1;
		index1 = tmp;
	}
	printf("index0 == %d %d\n", index0, curr_room->walls->len);
	if (index0 != -1)
	{
		if (++curr_room->walls->values[index0].normal_type == 3)
			curr_room->walls->values[index0].normal_type = 0;
	}
}

void			editor_tool_select(t_editor *editor, SDL_Event *event)
{
	const Uint8	*state = SDL_GetKeyboardState(NULL);
	int			index;

	if (state[SDL_SCANCODE_LCTRL] && event->button.button == SDL_BUTTON_LEFT 
		&& editor->grid_cell_grab == GG_LINE && editor->current_room != -1)
	{
		swap_normal(editor);
		return ;
	}

	if (editor->grid_cell_grab != GG_OUTSIDE)
	{
		if (editor->current_room != -1 && editor->grid_cell_grab == GG_LINE)
			editor->current_seg = get_close_seg(editor, &editor->rooms->values[editor->current_room], editor->grid_cell);
		else
			editor->current_seg = (t_vec2){ -1, -1 };
		select_room(editor, get_close_room(editor));
		editor_settings_update(editor);
	}
	if (editor->grid_cell_grab == GG_POINT)
	{
		index = vertices2d_indexof(editor->points, editor->grid_cell);
		if (index != -1)
			editor->current_point = index;
	}
}
