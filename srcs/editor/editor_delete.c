/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 22:38:06 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/12 13:53:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void	editor_remove_point(t_editor *editor)
{
	int		i;
	int		index;
	int		index0;
	t_bool	rem_room;

	if ((index = vertices2d_indexof(editor->points, editor->grid_cell)) == -1)
		return;
	rem_room = FALSE;
	i = -1;
	while (++i < editor->rooms->len)
	{
		if ((index0 = wall_indexof_by_indice(editor->rooms->values[i].walls, index)) == -1)
			continue;
		if (editor->rooms->values[i].walls->len - 1 < 3)
		{
			select_room(editor, -1);
			remove_room(editor, i);
			rem_room = TRUE;
			break;
		}
		else
			splice_walls_array(editor->rooms->values[i].walls, index0, 1);
	}
	if (!rem_room)
		remove_point(editor, index);
}

void	editor_delete_action(t_editor *editor)
{
	if (editor->grid_cell_grab == GG_POINT)
		editor_remove_point(editor);
	else if (editor->current_room != -1)
	{
		remove_room(editor, editor->current_room);
		select_room(editor, -1);
		editor->line_start_cell = (t_vec2){ -1, -1 };
	}
}
