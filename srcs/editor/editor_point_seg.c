/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_point_seg.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:03:42 by louali            #+#    #+#             */
/*   Updated: 2020/02/14 14:04:39 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec2			get_point_on_seg(t_vec2 p0, t_vec2 p1, t_vec2 pos)
{
	const	t_vec2	b = ft_vec2_sub(p1, p0);
	const	t_vec2	p = ft_vec2_sub(pos, p0);
	float			f;
	float			l;

	l = ft_vec2_len(b);
	f = clamp(0, 1, ft_vec2_dot(p, b) / l / l);
	return ((t_vec2){ p0.x + f * b.x, p0.y + f * b.y });
}

t_bool			is_point_on_seg(t_vec2 project, t_vec2 pos)
{
	return (ft_vec2_len(ft_vec2_sub(project, pos)) < 6);
}

t_vec2			get_close_seg(t_editor *editor, t_room *room, t_vec2 pos)
{
	int		i;
	t_vec2	p0;
	t_vec2	p1;
	t_vec2	project;

	i = -1;
	while (++i < room->walls->len - !(room->closed))
	{
		p0 = editor->points->vertices[room->walls->values[i].indice];
		p1 = editor->points->vertices[room->walls->values[(i + 1)
			% room->walls->len].indice];
		project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
			return ((t_vec2){ room->walls->values[i].indice,
				room->walls->values[(i + 1) % room->walls->len].indice });
	}
	return ((t_vec2){ -1, -1 });
}
