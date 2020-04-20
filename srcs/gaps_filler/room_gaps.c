/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_gaps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 00:46:52 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 23:59:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bool		update_rooms_gaps(t_editor *editor)
{
	int			i;

	i = -1;
	while (++i < editor->rooms->len)
		if (!get_room_gaps(editor, editor->rooms->values + i))
			return (FALSE);
	return (TRUE);
}

void		init_wall_len(t_wall *wall0, t_wall *wall1)
{
	if (wall0->start_rooms_range)
		wall0->start_rooms_range->len = 0;
	if (wall1->end_rooms_range)
		wall1->end_rooms_range->len = 0;
}

t_bool		walls_range(t_editor *editor, t_room *room, t_wall *wall0,
	t_wall *wall1)
{
	int		j;

	j = -1;
	while (++j < editor->rooms->len)
	{
		if (room == &editor->rooms->values[j])
			continue;
		if (range_wall(wall0, wall1, editor, j) == 0)
			continue;
		if (range_wall(wall0, wall1, editor, j) == -1)
			return (FALSE);
	}
	return (TRUE);
}

t_bool		get_room_gaps(t_editor *editor, t_room *room)
{
	int			i;
	t_wall		*wall0;
	t_wall		*wall1;

	if (!room->closed)
		return (TRUE);
	i = -1;
	while (++i < room->walls->len)
	{
		wall0 = &room->walls->values[i];
		wall1 = &room->walls->values[(i + 1) % room->walls->len];
		init_wall_len(wall0, wall1);
		if (!walls_range(editor, room, wall0, wall1))
			return (FALSE);
		if (wall0->start_rooms_range)
			sort_ranges(wall0->start_rooms_range);
		if (wall1->end_rooms_range)
			sort_ranges(wall1->end_rooms_range);
	}
	update_walls_sections(editor, room);
	return (TRUE);
}
