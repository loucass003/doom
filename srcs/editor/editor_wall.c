/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_wall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:54:05 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/08 20:16:53 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_wall			init_wall(int indice)
{
	if (indice < 0)
		return (t_wall){ .indice = indice };
	return (t_wall){
		.indice = indice,
		.floor_height = 0,
		.ceiling_height = 10
	};
}

int				wall_indexof_by_indice(t_walls *walls, int indice)
{
	int			i;

	i = -1;
	while (++i < walls->len)
	{
		if (walls->values[i].indice == indice)
			return (i);
	}
	return (-1);
}

void			editor_render_wall_nornal(t_doom *doom, t_room *room, t_line l,
	int j)
{
	int				color;
	t_vec2			dir;
	t_vec2			c;
	t_wall_section	ws;
	t_vec2			n;

	color = doom->editor.current_room != -1 && room ==
		&doom->editor.rooms->values[doom->editor.current_room]
		? 0xFFFFF0F0 : 0xFFFF9090;
	dir = ft_vec2_norm(ft_vec2_sub(l.a, l.b));
	c = ft_vec2_add(l.a, ft_vec2_mul_s(ft_vec2_sub(l.b, l.a), 0.5));
	ws = room->walls->values[j].wall_sections->values[
		doom->editor.wall_section];
	n = (t_vec2){ -dir.y, dir.x };
	if (ws.normal_type == 2 || ws.normal_type == 0)
	{
		draw_line(&doom->screen, (t_pixel){ c.x, c.y, color},
			(t_pixel){ c.x + n.x * 10, c.y + n.y * 10, 0 });
	}
	if (ws.normal_type == 1 || ws.normal_type == 2)
	{
		n = ft_vec2_inv(n);
		draw_line(&doom->screen, (t_pixel){ c.x, c.y, color},
			(t_pixel){ c.x + n.x * 10, c.y + n.y * 10, 0 });
	}
}

void			init_render(int *color, t_room *room,
	t_editor *editor)
{
	// int		test_room;

	*color = editor->current_room != -1 && room
		== &editor->rooms->values[editor->current_room] ? 0xFFFFF0F0 :
		0xFFFF9090;
	// test_room = point_in_rooms(editor, editor->grid_cell);
	// if (test_room != -1 && room == &editor->rooms->values[test_room])
	// 	*color = 0xFF00FF00;
}

t_bool			editor_render_wall(t_doom *doom, t_editor *editor, t_room *room,
	int j)
{
	t_line	p;
	int		color;

	init_render(&color, room, editor);
	p.a = editor->points->vertices[room->walls->values[j].indice];
	p.b = editor->points->vertices[
			room->walls->values[(j + 1) % room->walls->len].indice];
	if (editor->current_seg.x == room->walls->values[j].indice &&
		editor->current_seg.y == room->walls->values[(j + 1)
		% room->walls->len].indice)
		color = 0xFFFF0000;
	draw_line(&doom->screen, (t_pixel){ p.a.x, p.a.y, color},
		(t_pixel){ p.b.x, p.b.y, 0 });
	draw_circle(&doom->screen, (t_pixel){ p.a.x, p.a.y, 0xFFFF00FF }, 2);
	if (!room->closed || (p.a.x == p.b.x && p.a.y == p.b.y)
		|| (editor->current_room != -1
			&& room != &editor->rooms->values[editor->current_room]))
		return (FALSE);
	if (editor->wall_section != -1 && editor->current_seg.x ==
		room->walls->values[j].indice && editor->current_seg.y ==
		room->walls->values[(j + 1) % room->walls->len].indice)
		editor_render_wall_nornal(doom, room, (t_line){ .a = p.a,
		.b = p.b }, j);
	return (TRUE);
}
