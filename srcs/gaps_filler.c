/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gaps_filler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 13:36:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/19 02:17:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "arrays.h"
#include "editor.h"
#include "doom.h"

static void		swap(t_vec4 *a, t_vec4 *b)
{
	t_vec4 t;

	t = *a;
	*a = *b;
	*b = t;
}

void		sort_ranges(t_4dvertices *range)
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

t_bool			update_walls_sections(t_editor *editor, t_room *room)
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
		{
			printf("PROBLEM !\n");
			continue;
		}
		if (w0->start_rooms_range && w1->end_rooms_range && w0->start_rooms_range->len > 0 && w1->end_rooms_range->len > 0)
		{
			// printf ("-- ENTER\n");
			t_gap_filler_packet p = (t_gap_filler_packet){
				.start_a = w0->floor_height, .start_b = w1->floor_height };
			for (int k = 0; k < w0->start_rooms_range->len; k++)
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
			int		i;
			int		len = w0->wall_sections->len;

			w0->wall_sections->len = 0;
			i = -1;
			// printf("LEN (%d) %d %d\n", j, wall_sections->len, len);
			while (++i < wall_sections->len)
			{
				if (i >= len)
				{
					if (!append_wall_sections_array(&w0->wall_sections, wall_sections->values[i]))
						return (FALSE);
				}
				else
				{
					w0->wall_sections->len++;
					ft_memcpy(w0->wall_sections->values[i].vertices_index, wall_sections->values[i].vertices_index, sizeof(int) * 4);
				}
			}
			free(wall_sections);
			wall_sections = NULL;
		}
		else
			w0->wall_sections = wall_sections;
	}
	return (TRUE);
}

t_bool			update_rooms_gaps(t_editor *editor)
{
	int			i;

	i = -1;
	while (++i < editor->rooms->len)
		if (!get_room_gaps(editor, editor->rooms->values + i))
			return (FALSE);
	return (TRUE);
}

t_bool			get_room_gaps(t_editor *editor, t_room *room)
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
		if (wall0->start_rooms_range)
			wall0->start_rooms_range->len = 0;
		if (wall1->end_rooms_range)
			wall1->end_rooms_range->len = 0;
		while (++j < editor->rooms->len)
		{
			if (room == &editor->rooms->values[j])
				continue;
			t_room *curr_room = &editor->rooms->values[j];
			int index0 = wall_indexof_by_indice(curr_room->walls, wall0->indice);
			int index1 = wall_indexof_by_indice(curr_room->walls, wall1->indice);
			if (index0 == -1 || index1 == -1)
				continue;
			t_wall wall2 = editor->rooms->values[j].walls->values[index0];
			t_wall wall3 = editor->rooms->values[j].walls->values[index1];
			if ((wall2.floor_height > wall0->floor_height && wall2.floor_height < wall0->ceiling_height) 
				|| (wall2.ceiling_height > wall0->floor_height && wall2.ceiling_height < wall0->ceiling_height))
			{
				if (!wall0->start_rooms_range && !(wall0->start_rooms_range = create_4dvertices_array(5)))
					return (FALSE);
				append_4dvertices_array(&wall0->start_rooms_range, (t_vec4){ wall2.floor_height, wall2.ceiling_height, j, index0 });
			}
			if ((wall3.floor_height > wall1->floor_height && wall3.floor_height < wall1->ceiling_height) 
				|| (wall3.ceiling_height > wall1->floor_height && wall3.ceiling_height < wall1->ceiling_height))
			{
				if (!wall1->end_rooms_range && !(wall1->end_rooms_range = create_4dvertices_array(5)))
					return (FALSE);
				append_4dvertices_array(&wall1->end_rooms_range, (t_vec4){ wall3.floor_height, wall3.ceiling_height, j, index1 });
			}
		}
		if (wall0->start_rooms_range)
			sort_ranges(wall0->start_rooms_range);
		if (wall1->end_rooms_range)
			sort_ranges(wall1->end_rooms_range);
	}
	update_walls_sections(editor, room);
	return (TRUE);
}
