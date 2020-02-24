/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_point_room.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:02:10 by louali            #+#    #+#             */
/*   Updated: 2020/02/24 13:32:40 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			point_room(int index1, t_room *room0,
	int point_index, int index0)
{
	float	floor_h;
	float	ceil_h;
	int		index;

	append_walls_array(&room0->walls, init_wall(-20));
	floor_h = room0->walls->values[index1].floor_height;
	ceil_h = room0->walls->values[index1].ceiling_height;
	index = index1;
	if (index1 == room0->walls->len - 2 && index0 == 0)
		index = room0->walls->len - 1;
	else
		ft_memmove(room0->walls->values + index1 + 1,
			room0->walls->values + index1, (room0->walls->len
			- (index1)) * sizeof(t_wall));
	room0->walls->values[index] = init_wall(point_index);
	room0->walls->values[index].floor_height = floor_h;
	room0->walls->values[index].ceiling_height = ceil_h;
}

t_bool			init_p(t_editor *editor, t_room *room, int k)
{
	editor->grid_cell_grab = GG_LINE;
	editor->close_seg = (t_vec2){ room->walls->values[k].indice,
	room->walls->values[(k + 1) % room->walls->len].indice };
	return (TRUE);
}

t_vec2			point_on_room(t_editor *editor, t_room *room,
	t_vec2 pos, t_bool *found)
{
	int			k;
	t_vec2		project;
	t_vec2		p1;
	t_vec2		p0;

	*found = TRUE;
	k = -1;
	while (++k < room->walls->len - !(room->closed))
	{
		p0 = editor->points->vertices[room->walls->values[k].indice];
		p1 = editor->points->vertices[room->walls->values[(k + 1)
		% room->walls->len].indice];
		editor->grid_cell_grab = GG_POINT;
		if (is_in_range(p0, pos))
			return (p0);
		if (is_in_range(p1, pos))
			return (p1);
		project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
			if (init_p(editor, room, k))
				return (project);
	}
	*found = FALSE;
	return (pos);
}
