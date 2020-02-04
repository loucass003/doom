/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_wall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:54:05 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/04 00:24:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_wall			init_wall(int indice)
{
	if (indice < 0)
		return (t_wall){ .indice = indice };
	return  (t_wall){ 
		.indice = indice,
		.floor_height = 0,
		.ceiling_height = 10
	};
}

int			wall_indexof_by_indice(t_walls *walls, int indice)
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

void		editor_render_wall_nornal(t_doom *doom, t_room *room, t_line l, int j)
{
	const int				color = doom->editor.current_room != -1 && room
		== &doom->editor.rooms->values[doom->editor.current_room]
			? 0xFFFFF0F0 : 0xFFFF9090;
	const t_vec2			dir = ft_vec2_norm(ft_vec2_sub(l.a, l.b));
	const t_vec2			c = ft_vec2_add(l.a, 
		ft_vec2_mul_s(ft_vec2_sub(l.b, l.a), 0.5));
	const t_wall_section	ws = room->walls->values[j]
		.wall_sections->values[doom->editor.wall_section];
	t_vec2					n;
	
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

t_bool		editor_render_wall(t_doom *doom, t_editor *editor, t_room *room, int j)
{
	t_vec2	p0;
	t_vec2	p1;
	int	color = editor->current_room != -1 && room
		== &editor->rooms->values[editor->current_room] ? 0xFFFFF0F0 : 0xFFFF9090;
	
	int test_room = point_in_rooms(editor, editor->grid_cell);
	if (test_room != -1 && room == &editor->rooms->values[test_room])
		color = 0xFF00FF00;

	p0 = editor->points->vertices[room->walls->values[j].indice];
	p1 = editor->points->vertices[
			room->walls->values[(j + 1) % room->walls->len].indice];
	if (editor->current_seg.x == room->walls->values[j].indice && editor->current_seg.y == room->walls->values[(j + 1) % room->walls->len].indice)
		color = 0xFFFF0000;
	draw_line(&doom->screen, (t_pixel){ p0.x, p0.y, color},
		(t_pixel){ p1.x, p1.y, 0 });
	draw_circle(&doom->screen, (t_pixel){ p0.x, p0.y, 0xFFFF00FF }, 2);
	if (!room->closed || (p0.x == p1.x && p0.y == p1.y) 
		|| (editor->current_room != -1 
			&& room != &editor->rooms->values[editor->current_room]))
		return (FALSE);
	if (editor->wall_section != -1 && editor->current_seg.x == room->walls->values[j].indice && editor->current_seg.y == room->walls->values[(j + 1) % room->walls->len].indice)
		editor_render_wall_nornal(doom, room, (t_line){ .a = p0, .b = p1 }, j);
	return (TRUE);
}

t_wall			*get_current_wall(t_editor *editor)
{
	if (editor->current_room == -1)
		return (NULL);
	t_room	*room = &editor->rooms->values[editor->current_room];
	int wall_index = wall_indexof_by_indice(room->walls, editor->current_seg.x);
	if (wall_index == -1)
		return (NULL);
	return (&room->walls->values[wall_index]);
}

void					hide_doors_wall(t_editor *editor)
{
	int				i;
	int				j;
	int				k;
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;

	i = -1;
	while (++i < editor->rooms->len)
	{
		j = -1;
		room = &editor->rooms->values[i];
		while (++j < room->walls->len)
		{
			k = -1;
			wall = &room->walls->values[j];
			while (++k < wall->wall_sections->len)
			{
				ws = &wall->wall_sections->values[k];
				if (ws->type == WS_DOOR)
					hide_adjacent_walls(editor, i, j, ws);
			}
		}
	}
}

void					hide_adjacent_walls(t_editor *editor, int room, int wall, t_wall_section *ws)
{
	int		i;
	int		j;
	int		k;
	t_room			*r;
	t_wall			*w;
	t_wall			*w1;
	t_wall_section	*s;

	t_vec2 range3 = (t_vec2){ get_map(editor)->vertices->vertices[ws->vertices_index[0]].y, get_map(editor)->vertices->vertices[ws->vertices_index[2]].y };
	t_vec2 range4 = (t_vec2){ get_map(editor)->vertices->vertices[ws->vertices_index[1]].y, get_map(editor)->vertices->vertices[ws->vertices_index[3]].y };

	t_room t_r = editor->rooms->values[room];
	t_wall t_w = t_r.walls->values[wall];
	t_wall t_w1 = t_r.walls->values[(wall + 1) % t_r.walls->len];
	i = -1;
	while (++i < editor->rooms->len)
	{
		j = -1;
		r = &editor->rooms->values[i];
		while (++j < r->walls->len)
		{
			w = &r->walls->values[j];
			w1 = &r->walls->values[(j + 1) % r->walls->len];
			if ((w->indice == t_w.indice && w1->indice == t_w1.indice) || (w->indice == t_w1.indice && w1->indice == t_w.indice))
			{
				k = -1;
				while (++k < w->wall_sections->len)
				{
					s = &w->wall_sections->values[k];
					t_vec2 range1 = (t_vec2){ get_map(editor)->vertices->vertices[s->vertices_index[0]].y, get_map(editor)->vertices->vertices[s->vertices_index[2]].y };
					t_vec2 range2 = (t_vec2){ get_map(editor)->vertices->vertices[s->vertices_index[1]].y, get_map(editor)->vertices->vertices[s->vertices_index[3]].y };
					if (range1.x == range3.x && range1.y == range3.y 
						&& range2.x == range4.x && range2.y == range4.y)
					{
						s->invisible = TRUE;
						s->collisions = FALSE;
					}
				}
			}
		}
	}
}