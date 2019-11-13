/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_intersect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:36:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/10 18:37:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int			orientation(t_vec2 p, t_vec2 q, t_vec2 r) 
{
	int val = (q.y - p.y) * (r.x - q.x) - 
				(q.x - p.x) * (r.y - q.y); 

	if (val == 0) return 0;
	return (val > 0) ? 1: 2;
} 

t_bool		seg_intersect(t_line p, t_line q, t_bool colinear_check)
{ 
    int o1 = orientation(p.a, p.b, q.a); 
    int o2 = orientation(p.a, p.b, q.b); 
    int o3 = orientation(q.a, q.b, p.a); 
    int o4 = orientation(q.a, q.b, p.b); 

	if ((q.a.x == p.b.x && q.a.y == p.b.y) || (q.b.x == p.a.x && q.b.y == p.a.y))
		return (FALSE);
	if (o1 != o2 && o3 != o4)
		return TRUE;
	return FALSE;
}

t_bool		get_line_intersection(t_line a, t_line b, t_bool strict)
{
	t_vec2	s1;
	t_vec2	s2;

	s1.x = a.b.x - a.a.x;     s1.y = a.b.y - a.a.y;
	s2.x = b.b.x - b.a.x;     s2.y = b.b.y - b.a.y;

	float s, t;
	s = (-s1.y * (a.a.x - b.a.x) + s1.x * (a.a.y - b.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (a.a.y - b.a.y) - s2.y * (a.a.x - b.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (strict && s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return (TRUE);
	else if (s > 0 && s < 1 && t > 0 && t < 1)
		return (TRUE);
	return (FALSE);
}


t_bool			room_intersect(t_editor *editor, t_room *room, t_room *room2, t_bool strict)
{
	int		i;
	int		j;
	int		k;
	t_line	line1;
	t_line	line2;

	i = -1;
	while (++i < room->walls->len - 1)
	{
		t_vec2 p0 = editor->points->vertices[room->walls->values[i].indice];
		t_vec2 p1 = editor->points->vertices[room->walls->values[i + 1].indice];
		line1 = (t_line){ .a = p0, .b = p1 };
		k = -1;
		while (++k < room2->walls->len - !(room2->closed))
		{
			t_vec2 p0 = editor->points->vertices[room2->walls->values[k].indice];
			t_vec2 p1 = editor->points->vertices[room2->walls->values[(k + 1) % room2->walls->len].indice];
			line2 = (t_line){ .a = p0, .b = p1 };
			if (line1.a.x == line2.a.x && line1.a.y == line2.a.y && line1.b.x == line2.b.x && line1.b.y == line2.b.y)
				continue;
			if (line1.b.x == line2.a.x && line1.b.y == line2.a.y)
				continue;
			if (line2.b.x == line1.a.x && line2.b.y == line1.a.y)
				continue;
			if (get_line_intersection(line1, line2, strict))
				return (TRUE);
		}
	}
	return (FALSE);
}