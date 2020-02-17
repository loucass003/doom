/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_rooms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:34:01 by louali            #+#    #+#             */
/*   Updated: 2020/02/14 14:34:58 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include <limits.h>

void		editor_render_rooms(t_doom *doom, t_editor *editor)
{
	int		i;
	t_room	*room;
	t_vec2	first_point;
	int		j;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (room->walls->len < 2)
			continue ;
		j = -1;
		if (!room->closed)
		{
			first_point = editor->points->vertices[room->walls
			->values[0].indice];
			draw_circle(&doom->screen, (t_pixel){ first_point.x,
				first_point.y, 0xFFFFF00F }, 6);
		}
		while (++j < room->walls->len - !(room->closed))
			if (!editor_render_wall(doom, editor, room, j))
				continue ;
	}
}

int			point_in_rooms(t_editor *editor, t_vec2 point)
{
	int			i;
	t_room		*room;
	int			index;

	i = -1;
	index = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (point_in_room(editor, room, point))
			index = i;
	}
	return (index);
}

void		free_rooms(t_rooms **rooms)
{
	int		i;
	t_room	*room;

	if (!*rooms)
		return ;
	i = -1;
	while (++i < (*rooms)->len)
	{
		room = &(*rooms)->values[i];
		free_walls(&room->walls);
	}
	ft_memdel((void **)rooms);
}
