/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:38:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/16 19:43:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	intersect(t_line *line, t_line *seg, t_vec2 *intersect)
{
	t_vec2 i1 = (t_vec2) {
		line->b.y - line->a.y,
		line->a.x - line->b.x
	};
	float c1 = i1.x * (line->a.x) + i1.y * (line->a.y);

	t_vec2 i2 = (t_vec2) {
		seg->b.y - seg->a.y,
		seg->a.x - seg->b.x
	};
	float c2 = i2.x * (seg->a.x) + i2.y * (seg->a.y);
	float d = i1.x * i2.y - i2.x * i1.y;
	if (d == 0)
	{
		intersect->x = -1;
		intersect->y = -1;
		return (FALSE);
	}
	intersect->x = (i2.y * c1 - i1.y * c2) / d;
	intersect->y = (i1.x * c2 - i2.x * c1) / d;
	return (TRUE);
}

t_vec2	line_normal(t_line *line)
{
	return (ft_vec2_norm((t_vec2) {
		line->b.y - line->a.y,
		line->a.x - line->b.x
	}));
}

t_side	get_side_thin(t_line *partition, t_vec2 seg)
{
	const t_vec2 n = line_normal(partition);
	const float side = (seg.x - partition->a.x) * n.x
	+ (seg.y - partition->a.y) * n.y;

	if (side == 0)
		return (S_COLINEAR);
	return (side > 0 ? S_BACK : S_FRONT);
}

t_side	get_side_thick(t_line *partition, t_vec2 seg)
{
	t_side			b_side;
	const t_vec2	n = line_normal(partition);
	const t_side	f_side = get_side_thin(partition, (t_vec2){ seg.x + n.x / 2, seg.y + n.y / 2 });

	if (f_side == S_FRONT)
		return (S_FRONT);
	else if (f_side == S_BACK)
	{
		b_side = get_side_thin(partition,
			(t_vec2){ seg.x - n.x / 2, seg.y - n.y / 2 });
		if (b_side == S_BACK)
			return (S_BACK);
	}
	return (S_COLINEAR);
}

t_side	get_side(t_line *partition, t_line seg)
{
	t_side	s1;
	t_side	s2;

	if (partition->a.x == seg.a.x && partition->a.y == seg.a.y
		&& partition->b.x == seg.b.x && partition->b.y == seg.b.y)
		return (S_COLINEAR);
	s1 = get_side_thick(partition, seg.a);
	s2 = get_side_thick(partition, seg.b);
	if (s1 == s2)
		return (s1);
	else if (s1 == S_COLINEAR)
		return (s2);
	else if (s2 == S_COLINEAR)
		return (s1);
	else
		return (S_SPANNING);
}
