/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_sections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 00:51:17 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 00:51:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bool			arrange_wall_sections(t_wall *w0,
	t_wall_sections *wall_sections)
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

void		append_middle_gaps(t_editor *editor, t_gap_filler_packet *p,
	t_wall_sections **wall_sections, int j)
{
	int k;

	k = -1;
	while (++k < p->w0->start_rooms_range->len)
	{
		p->range_a = p->w0->start_rooms_range->vertices[k];
		p->range_b = p->w1->end_rooms_range->vertices[k];
		if (p->start_a < p->range_a.x && p->start_b < p->range_b.x
			&& p->start_a < p->w0->ceiling_height
				&& p->start_b < p->w1->ceiling_height)
			append_wall_sections_array(wall_sections,
				create_gap_wall_section(editor, p->room, j, *p));
		append_wall_sections_array(wall_sections,
			create_between_gaps_wall_section(editor, p->room, j, *p));
		p->start_a = p->range_a.y;
		p->start_b = p->range_b.y;
		p->last_range_a = p->range_a;
		p->last_range_b = p->range_b;
	}
}

void		add_wall_sections(t_editor *editor, t_wall_sections **wall_sections,
	t_gap_filler_packet *p, int j)
{
	if (p->w0->start_rooms_range && p->w1->end_rooms_range
			&& p->w0->start_rooms_range->len > 0
			&& p->w1->end_rooms_range->len > 0)
	{
		append_middle_gaps(editor, p, wall_sections, j);
		if (p->start_a < p->w0->ceiling_height
			&& p->start_a > p->w0->floor_height
			&& p->start_b < p->w1->ceiling_height
			&& p->start_b > p->w1->floor_height)
			append_wall_sections_array(wall_sections,
				create_last_wall_section(editor, p->room, j, *p));
	}
	else
		append_wall_sections_array(wall_sections,
			create_simple_wall_section(editor, p->room, j));
}

t_bool		assing_wall_sections(t_wall_sections **wall_sections, t_wall *w0)
{
	if (w0->wall_sections)
	{
		if (!arrange_wall_sections(w0, *wall_sections))
			return (FALSE);
		ft_memdel((void **)wall_sections);
	}
	else
		w0->wall_sections = *wall_sections;
	return (TRUE);
}

t_bool		update_walls_sections(t_editor *editor, t_room *room)
{
	int					j;
	t_wall				*w0;
	t_wall				*w1;
	t_wall_sections		*wall_sections;
	t_gap_filler_packet	p;

	j = -1;
	while (++j < room->walls->len)
	{
		w0 = &room->walls->values[j];
		w1 = &room->walls->values[((j + 1) % room->walls->len)];
		p = (t_gap_filler_packet){ .start_a = w0->floor_height,
			.start_b = w1->floor_height, .w0 = w0, .w1 = w1, .room = room };
		if (!(wall_sections = create_wall_sections_array(5)))
			return (FALSE);
		if (w0->start_rooms_range && w1->end_rooms_range
			&& w0->start_rooms_range->len != w1->end_rooms_range->len)
			continue;
		add_wall_sections(editor, &wall_sections, &p, j);
		if (!assing_wall_sections(&wall_sections, w0))
			return (FALSE);
	}
	return (TRUE);
}

