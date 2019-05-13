/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/13 01:32:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define EPSILON (1e-3)

static void		swap_vertices_axis(t_polygon *poly)
{
	int		i;
	float	tmp;
	t_vec3 	*v;

	if (poly->type == P_WALL)
	{
		i = -1;
		while (++i < poly->vertices->len)
		{
			v = &poly->vertices->vertices[i];
			tmp = v->y;
			v->y = v->z;
			v->z = tmp;
		}
	}
}

static float	area(t_3dvertices *vertices)
{
/*	float	a;
	int		p;
	int		q;

	a = 0;
	p = vertices->len - 1;
	q = 0;
	while (q < vertices->len)
	{
		a += vertices->vertices[p].x * vertices->vertices[q].y
			- vertices->vertices[q].x * vertices->vertices[p].y;
		p = q++;
	}
	return (a * 0.5);*/
	int n = vertices->len;
	float A = 0.0f;
	for (int p = n - 1, q = 0; q < n; p = q++) {
		t_vec3 pval = vertices->vertices[p];
		t_vec3 qval = vertices->vertices[q];
		A += pval.x * qval.y - qval.x * pval.y;
	}
	return (A * 0.5f);
}

static t_bool	inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p)
{
	float	i = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
	float	j = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
	float	k = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);

	return (i >= 0 && j >= 0 && k >= 0);
}

static t_bool	snip(t_3dvertices *vertices, int u, int j, int w, int n, int *v)
{
	int		p;
	t_vec3	a, b, c;

	a = vertices->vertices[v[u]];
	b = vertices->vertices[v[j]];
	c = vertices->vertices[v[w]];
	float t = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
	if (EPSILON > t)
		return (FALSE);
	p = -1;
	while (++p < n)
	{
		if (p == u || p == j || p == w)
			continue;
		if (inside_triangle(a, b, c, vertices->vertices[v[p]]))
			return (FALSE);
	}
	return (TRUE);
}

t_bool	triangulate_polygon(t_polygon *polygon)
{
	int		*v;
	float	a;
	int		i;

	if (polygon->vertices->len < 3 
		|| !(v = (int *)malloc(polygon->vertices->len * sizeof(int))))
		return (FALSE);
	swap_vertices_axis(polygon);
	polygon->indices = create_ints_array(polygon->vertices->len * 3);
	a = area(polygon->vertices) > 0;
	i = -1;
	while (++i < polygon->vertices->len)
		v[i] = a ? i : (polygon->vertices->len - 1) - i;
	int nv = polygon->vertices->len;
	int count = 2 * nv;
	for (int j = nv - 1; nv > 2; )
	{
		if ((count--) <= 0)
		{
			swap_vertices_axis(polygon);
			return (TRUE);
		}
		int u = j;
		if (nv <= u)
			u = 0;
		j = u + 1;
		if (nv <= j)
			j = 0;
		int w = j + 1;
		if (nv <= w)
			w = 0;
		if (snip(polygon->vertices, u, j, w, nv, v))
		{
			int s, t;
			append_ints_array(&polygon->indices, v[u]);
			append_ints_array(&polygon->indices, v[j]);
			append_ints_array(&polygon->indices, v[w]);
			for (s = j, t = j + 1; t < nv; s++, t++)
				v[s] = v[t];
			nv--;
			count = 2 * nv;
		}	
	}
	swap_vertices_axis(polygon);
	return (TRUE);
}

void	triangulate_bsp(t_node *n)
{
	int	i;

	if (!n)
		return ;
	i = -1;
	while (++i < n->polygons->len)
		triangulate_polygon(&n->polygons->polygons[i]);
	triangulate_bsp(n->front);
	triangulate_bsp(n->back);
}