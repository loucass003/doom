/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_hide_wall.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:05:38 by louali            #+#    #+#             */
/*   Updated: 2020/02/24 15:34:17 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void		hide_doors_wall(t_editor *editor)
{
	int				index[3];
	t_room			*room;
	t_wall			*wall;
	t_wall_section	*ws;

	index[0] = -1;
	while (++index[0] < editor->rooms->len)
	{
		index[1] = -1;
		room = &editor->rooms->values[index[0]];
		while (++index[1] < room->walls->len)
		{
			index[2] = -1;
			wall = &room->walls->values[index[1]];
			while (++index[2] < wall->wall_sections->len)
			{
				ws = &wall->wall_sections->values[index[2]];
				if (ws->type == WS_DOOR)
					hide_adjacent_walls(editor, index[0], index[1], ws);
			}
		}
	}
}

void		hide_wall(t_editor *editor, t_line range, int k,
	t_wall *wa)
{
	t_vec2			range2;
	t_vec2			range1;
	t_wall_section	*s;

	s = &wa->wall_sections->values[k];
	range1 = (t_vec2){ get_map(editor)->vertices->vertices[
		s->vertices_index[0]].y,
		get_map(editor)->vertices->vertices[
		s->vertices_index[2]].y };
	range2 = (t_vec2){ get_map(editor)->vertices->vertices[
		s->vertices_index[1]].y,
		get_map(editor)->vertices->vertices[
		s->vertices_index[3]].y };
	if (range1.x == range.a.x && range1.y == range.a.y
		&& range2.x == range.b.x && range2.y == range.b.y)
	{
		s->invisible = TRUE;
		s->collisions = FALSE;
	}
}

void		hide_walls(t_wall2 t_w, t_editor *editor,
	t_line range, int i)
{
	int		j;
	t_room	*r;
	int		k;
	t_wall	*wa;
	t_wall	*wb;

	j = -1;
	r = &editor->rooms->values[i];
	while (++j < r->walls->len)
	{
		wa = &r->walls->values[j];
		wb = &r->walls->values[(j + 1) % r->walls->len];
		if ((wa->indice == t_w.a.indice && wb->indice == t_w.b.indice)
			|| (wa->indice == t_w.b.indice && wb->indice == t_w.a.indice))
		{
			k = -1;
			while (++k < wa->wall_sections->len)
				hide_wall(editor, range, k, wa);
		}
	}
}

void		hide_adjacent_walls(t_editor *editor, int room,
	int wall, t_wall_section *ws)
{
	int				i;
	t_line			range;
	t_wall2			t_w;
	t_room			t_r;

	range.a = (t_vec2){ get_map(editor)->vertices->vertices[
		ws->vertices_index[0]].y,
		get_map(editor)->vertices->vertices[ws->vertices_index[2]].y };
	range.b = (t_vec2){ get_map(editor)->vertices->vertices[
		ws->vertices_index[1]].y,
		get_map(editor)->vertices->vertices[ws->vertices_index[3]].y };
	t_r = editor->rooms->values[room];
	t_w.a = t_r.walls->values[wall];
	t_w.b = t_r.walls->values[(wall + 1) % t_r.walls->len];
	i = -1;
	while (++i < editor->rooms->len)
		hide_walls(t_w, editor, range, i);
}

t_wall		*get_current_wall(t_editor *editor)
{
	int		wall_index;
	t_room	*room;

	if (editor->current_room == -1)
		return (NULL);
	room = &editor->rooms->values[editor->current_room];
	wall_index = wall_indexof_by_indice(room->walls, editor->current_seg.x);
	if (wall_index == -1)
		return (NULL);
	return (&room->walls->values[wall_index]);
}
