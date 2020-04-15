/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gaps_filler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 13:36:42 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 19:07:16 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "arrays.h"
#include "editor.h"
#include "doom.h"

 //TESTER CE FICHIER

static void		swap(t_vec4 *a, t_vec4 *b)
{
	t_vec4 t;

	t = *a;
	*a = *b;
	*b = t;
}

void			sort_ranges(t_4dvertices *range)
{
	int		i;
	int		j;
	t_vec4	*v0;
	t_vec4	*v1;

	i = -1;
	while (++i < range->len - 1)
	{
		j = -1;
		while (++j < range->len - i - 1)
		{
			v0 = &range->vertices[j];
			v1 = &range->vertices[j + 1];
			if (v1->x < v0->x)
				swap(v0, v1);
		}
	}
}

t_bool			arrange_wall_sections(t_wall *w0, t_wall_sections *wall_sections)
{
	int		i;
	int		len;

	len = w0->wall_sections->len;
	w0->wall_sections->len = 0;
	i = -1;
	while (++i < wall_sections->len)
	{
		if (i >= len)
		{
			if (!append_wall_sections_array(&w0->wall_sections,
				wall_sections->values[i]))
				return (FALSE);
		}
		else
		{
			w0->wall_sections->len++;
			ft_memcpy(w0->wall_sections->values[i].vertices_index,
				wall_sections->values[i].vertices_index, sizeof(int) * 4);
		}
	}
	return (TRUE);
}

t_bool		update_walls_sections(t_editor *editor, t_room *room)
{
	int					j;
	t_wall				*w0;
	t_wall				*w1;
	int					next;
	t_wall_sections		*wall_sections;

	j = -1;
	while (++j < room->walls->len)
	{
		next = ((j + 1) % room->walls->len);
		w0 = &room->walls->values[j];
		w1 = &room->walls->values[next];
		if (!(wall_sections = create_wall_sections_array(5)))
			return (FALSE);
		if (w0->start_rooms_range && w1->end_rooms_range
			&& w0->start_rooms_range->len != w1->end_rooms_range->len)
			continue;
		if (w0->start_rooms_range && w1->end_rooms_range
			&& w0->start_rooms_range->len > 0 && w1->end_rooms_range->len > 0)
		{
			t_gap_filler_packet p;
			p = (t_gap_filler_packet){
				.start_a = w0->floor_height, .start_b = w1->floor_height };
			int k;

			k = -1;
			while (++k < w0->start_rooms_range->len)
			{
				p.range_a = w0->start_rooms_range->vertices[k];
				p.range_b = w1->end_rooms_range->vertices[k];
				if (p.start_a < p.range_a.x && p.start_b < p.range_b.x
					&& p.start_a < w0->ceiling_height
						&& p.start_b < w1->ceiling_height)
					append_wall_sections_array(&wall_sections,
						create_gap_wall_section(editor, room, j, p));
				append_wall_sections_array(&wall_sections,
					create_between_gaps_wall_section(editor, room, j, p));
				p.start_a = p.range_a.y;
				p.start_b = p.range_b.y;
				p.last_range_a = p.range_a;
				p.last_range_b = p.range_b;
			}
			if (p.start_a < w0->ceiling_height && p.start_a > w0->floor_height
			&& p.start_b < w1->ceiling_height && p.start_b > w1->floor_height)
				append_wall_sections_array(&wall_sections,
					create_last_wall_section(editor, room, j, p));
		}
		else
			append_wall_sections_array(&wall_sections,
				create_simple_wall_section(editor, room, j));

		if (w0->wall_sections)
		{
			if (!arrange_wall_sections(w0, wall_sections))
				return (FALSE);
			free(wall_sections);
			wall_sections = NULL;
		}
		else
			w0->wall_sections = wall_sections;
	}
	return (TRUE);
}

t_bool		update_rooms_gaps(t_editor *editor)
{
	int			i;

	i = -1;
	while (++i < editor->rooms->len)
		if (!get_room_gaps(editor, editor->rooms->values + i))
			return (FALSE);
	return (TRUE);
}

t_bool		is_start_range(t_wall wall2, t_wall *wall0)
{
	return (wall2.floor_height > wall0->floor_height
		&& wall2.floor_height < wall0->ceiling_height)
		|| (wall2.ceiling_height > wall0->floor_height
		&& wall2.ceiling_height < wall0->ceiling_height);
}

t_bool		is_unrange(t_wall wall3, t_wall *wall1)
{
	return (wall3.floor_height > wall1->floor_height
		&& wall3.floor_height < wall1->ceiling_height)
		|| (wall3.ceiling_height > wall1->floor_height
		&& wall3.ceiling_height < wall1->ceiling_height);
}

int		range_wall(t_wall *wall0, t_wall *wall1, t_editor *editor, int j)
{
	t_room	*curr_room;
	int		index0;
	int		index1;
	t_wall	wall2;
	t_wall	wall3;

	curr_room = &editor->rooms->values[j];
	index0 = wall_indexof_by_indice(curr_room->walls, wall0->indice);
	index1 = wall_indexof_by_indice(curr_room->walls, wall1->indice);
	if (index0 == -1 || index1 == -1)
		return (0);
	wall2 = editor->rooms->values[j].walls->values[index0];
	wall3 = editor->rooms->values[j].walls->values[index1];
	if (is_start_range(wall2, wall0))
	{
		if (!wall0->start_rooms_range
			&& !(wall0->start_rooms_range = create_4dvertices_array(5)))
			return (-1);
		append_4dvertices_array(&wall0->start_rooms_range, (t_vec4){
				wall2.floor_height, wall2.ceiling_height, j, index0 });
	}
	if (is_unrange(wall3, wall1))
	{
		if (!wall1->end_rooms_range
			&& !(wall1->end_rooms_range = create_4dvertices_array(5)))
			return (-1);
		append_4dvertices_array(&wall1->end_rooms_range, (t_vec4){
			wall3.floor_height, wall3.ceiling_height, j, index1 });
	}
	return (1);
}

void		init_wall_len(t_room *room, t_wall *wall0, t_wall *wall1, int i)
{
	if (wall0->start_rooms_range)
		wall0->start_rooms_range->len = 0;
	if (wall1->end_rooms_range)
		wall1->end_rooms_range->len = 0;
}

t_bool		get_room_gaps(t_editor *editor, t_room *room)
{
	int			i;
	int			j;
	t_wall		*wall0;
	t_wall		*wall1;

	if (!room->closed)
		return (TRUE);
	i = -1;
	while (++i < room->walls->len)
	{
		j = -1;
		wall0 = &room->walls->values[i];
		wall1 = &room->walls->values[(i + 1) % room->walls->len];
		init_wall_len(room, wall0, wall1, i);
		while (++j < editor->rooms->len)
		{
			if (room == &editor->rooms->values[j])
				continue;
			if (range_wall(wall0, wall1, editor, j) == 0)
				continue;
			if (range_wall(wall0, wall1, editor, j) == -1)
				return (FALSE);
		}
		if (wall0->start_rooms_range)
			sort_ranges(wall0->start_rooms_range);
		if (wall1->end_rooms_range)
			sort_ranges(wall1->end_rooms_range);
	}
	update_walls_sections(editor, room);
	return (TRUE);
}
