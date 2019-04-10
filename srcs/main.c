/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:54:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/10 19:24:08 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "libft.h"
#include <math.h>
#include <limits.h>

typedef enum		e_side
{
	S_COLINEAR,
	S_FRONT,
	S_BACK
}					t_side;



typedef struct		s_line
{
	t_vec2			a;
	t_vec2			b;
//	uint8_t			side_sign;
}					t_line;

typedef struct		s_rect_data
{
	t_line			ab;
	t_line			ac;
	t_line			cd;
	t_line			bd;
}					t_rect_data;

typedef union		u_rect
{
	t_line			a[4];
	t_rect_data		d;
}					t_rect;

typedef struct		s_node
{
	t_line			partition;
	t_line			segment;
	struct s_node	*front;
	struct s_node	*back;
}					t_node;

t_line lines[] = {
	{ { 0, 150 }, { 0, 450 } },
	{ { 0, 450 }, { 800, 450 } },
	{ { 800, 450 }, { 800, 300 } },
	{ { 800, 300 }, { 500, 300 } },
	{ { 500, 300 }, { 500, 75 } },
	{ { 500, 75 }, { 0, 150 } }
};

t_bool	intersect(t_line *a, t_line *b, t_vec2 *intersect)
{
	if (a->a.x == b->a.x && a->a.y == b->a.y && a->b.x == b->b.x && a->b.y == b->b.y)
		return (FALSE);
	t_vec2 i1 = (t_vec2) {
		a->b.y - a->a.y,
		a->a.x - a->b.x
	};
	float c1 = i1.x * (a->a.x) + i1.y * (a->a.y);

	t_vec2 i2 = (t_vec2) {
		b->b.y - b->a.y,
		b->a.x - b->b.x
	};
	float c2 = i2.x * (b->a.x) + i2.y * (b->a.y);
	float d = i1.x * i2.y - i2.x * i1.y;
	if (d == 0)
		return (FALSE);
	intersect->x = (i2.y * c1 - i1.y * c2) / d;
	intersect->y = (i1.x * c2 - i2.x * c1) / d;
	return (TRUE);
}

void	get_line_sides(t_line *line)
{
	t_vec2 i1 = {
		line->a.x + (line->b.y - line->a.y),
		line->a.y + -(line->b.x - line->a.x)
	};
	float d = (i1.x - line->b.x) * (line->b.y - line->a.y)
	- (i1.y - line->a.y) * (line->b.x - line->a.x);

	t_vec2 i2 = {
		line->a.x + -(line->b.y - line->a.y),
		line->a.y + (line->b.x - line->a.x)
	};
	float d2 = (i2.x - line->b.x) * (line->b.y - line->a.y)
	- (i2.y - line->a.y) * (line->b.x - line->a.x);
	printf("(%f, %f) (%f, %f) (%f, %f)\n", i1.x, i1.y, i2.x, i2.y, d, d2);
}

/*

TODO: 
- Savoir si un polygone est un polygone ferme et ou croise ? (Pas obligatoire pour le moment) (https://stackoverflow.com/a/19180165)
- Savoir si un point est a l interieur d un polygone (https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon)
- Savoir quel cote d une droite est un FRONT/BACK (https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located) (pourrai aussi etre calcule avec l editeur de map)

Iterer sur le polygone
	Tester un point a l exterieur et a l interieur de chaque segments est garder le sigine de d du point a l interieur

d va aussi servir a savoir de quel cote sont les segements des autres sections
*/


t_side	get_sides(t_line *partition, t_line *seg)
{
	const float dot = ft_vec2_dot(
		ft_vec2_norm((t_vec2) { partition->b.x - partition->a.x, partition->b.y - partition->a.y }),
		ft_vec2_norm((t_vec2) { seg->b.x - seg->a.x, seg->b.y - seg->a.y }));
	if (dot == 0)
		return (S_COLINEAR);
	return (dot < 0 ? S_BACK : S_FRONT);
}

t_bool	outside_rect(t_vec2 *v, t_rect *r)
{
	return (v->x < r->d.ab.a.x || v->x > r->d.ab.b.x
		|| v->y < r->d.ab.a.y || v->y > r->d.bd.b.y);
}

t_line	get_partition(t_line *seg, t_line *lines, t_rect *bounds)
{
	int	lines_count = 6;
	int i = -1;
	printf("%d\n", lines_count);
	while (++i < lines_count)
	{
		t_vec2 it;
		if (intersect(seg, lines + i, &it) == FALSE)
			continue;
		if ((seg->a.x == it.x && seg->a.y == it.y) || (seg->b.x == it.x && seg->b.y == it.y))
			continue;
		
		printf("%d, %f, %f\n", outside_rect(&it, bounds), it.x, it.y);
	}
	
}

void	divide_seg(t_line *seg, t_line *front_seg, t_line *back_seg)
{

}

t_node	*build_node(t_line *lines)
{

}

t_rect	new_rect(t_vec2 min, t_vec2 max)
{
	return ((t_rect)((t_rect_data){
		(t_line){ min, (t_vec2){ max.x, min.y }},
		(t_line){ min, (t_vec2){ min.x, max.y }},
		(t_line){ (t_vec2){ min.x, max.y }, max},
		(t_line){ (t_vec2){ max.x, min.y }, max},
	}));
}

int		main(void)
{
	t_vec2 max = {INT_MIN, INT_MIN};
	t_vec2 min = {INT_MAX, INT_MAX};
	int count = sizeof(lines) / sizeof(t_line);
	for (int i = 0; i < count; i++)
	{
		max.x = fmax(max.x, fmax(lines[i].a.x, lines[i].b.x));
		max.y = fmax(max.y, fmax(lines[i].a.y, lines[i].b.y));
		min.x = fmin(min.x, fmin(lines[i].a.x, lines[i].b.x));
		min.y = fmin(min.y, fmin(lines[i].a.y, lines[i].b.y));
	}
	printf("(%f, %f) (%f, %f)\n", min.x, min.y, max.x, max.y);
	t_rect r = new_rect(min, max);
	get_partition(&lines[5], lines, &r);
	return (0);
}
