/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:38:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/25 17:01:39 by lloncham         ###   ########.fr       */
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

void	draw_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int	index;

	d = (t_pixel){ ft_abs(p1.x - p0.x), ft_abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		if (p0.x >= 0 && p0.x < img->width && p0.y >= 0 && p0.y < img->height)
			img->pixels[index = p0.y * img->width + p0.x] = p0.color;
		e[1] = e[0];
		if (e[1] > -d.x)
		{
			e[0] -= d.y;
			p0.x += s.x;
		}
		if (e[1] < d.y)
		{
			e[0] += d.x;
			p0.y += s.y;
		}
	}
}
