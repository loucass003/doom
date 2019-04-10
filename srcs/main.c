/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:54:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/10 02:22:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "libft.h"
#include <math.h>
#include <limits.h>

typedef struct		s_line
{
	t_vec2			a;
	t_vec2			b;
}					t_line;

typedef struct		s_node
{
	t_line			line;
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

t_bool	intersect(t_line a, t_line b, t_vec2 *intersect)
{
	if (a.a.x == b.a.x && a.a.y == b.a.y && a.b.x == b.b.x && a.b.y == b.b.y)
		return (FALSE);
	t_vec2 i1 = (t_vec2) {
		a.b.y - a.a.y,
		a.a.x - a.b.x
	};
	float c1 = i1.x * (a.a.x) + i1.y * (a.a.y);

	t_vec2 i2 = (t_vec2) {
		b.b.y - b.a.y,
		b.a.x - b.b.x
	};
	float c2 = i2.x * (b.a.x) + i2.y * (b.a.y);
	float d = i1.x * i2.y - i2.x * i1.y;
	if (d == 0)
		return (FALSE);
	intersect->x = (i2.y * c1 - i1.y * c2) / d; 
	intersect->y = (i1.x * c2 - i2.x * c1) / d; 
	return (TRUE);
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

int		main(void)
{
	t_vec2 i;
	t_bool f = intersect(lines[1], lines[4], &i);
	printf("%d %f, %f\n", f, i.x, i.y);
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
	return (0);
}
