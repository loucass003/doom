/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 00:10:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <maths.h>
#include <stdlib.h>
#include "polygon.h"
#include "maths/mat4.h"
#include "arrays.h"

#define EPSILON (1e-6)

static float	area(t_3dvertices *vertices)
{
	int n = vertices->len;
	float A = 0.0f;
	for (int p = n - 1, q = 0; q < n; p = q++) {
		t_vec3 pval = vertices->vertices[p];
		t_vec3 qval = vertices->vertices[q];
		A += pval.x * qval.y - qval.x * pval.y;
	}
	return (A * 0.5f);
}

t_bool	inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p)
{
	float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	float cCROSSap, bCROSScp, aCROSSbp;
 
	ax = c.x - b.x; ay = c.y - b.y;
	bx = a.x - c.x; by = a.y - c.y;
	cx = b.x - a.x; cy = b.y - a.y;
	apx = p.x - a.x; apy = p.y - a.y;
	bpx = p.x - b.x; bpy = p.y - b.y;
	cpx = p.x - c.x; cpy = p.y - c.y;

	aCROSSbp = ax * bpy - ay * bpx;
	cCROSSap = cx * apy - cy * apx;
	bCROSScp = bx * cpy - by * cpx;
 
	return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
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
	for (p = 0; p < n; p++) 
	{
		if (p == u || p == j || p == w)
			continue;
		if (inside_triangle(a, b, c, vertices->vertices[v[p]]))
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	ear_clip_polygon(t_polygon *polygon)
{
	int		*v;

	if (polygon->vertices->len < 3
		|| !(v = (int *)malloc(polygon->vertices->len * sizeof(int)))
		|| (!polygon->indices && !(polygon->indices = create_ints_array(polygon->vertices->len * 3))))
		return (FALSE);
	if (area(polygon->vertices) > 0) {
		for (int i = 0; i < polygon->vertices->len; i++)
			v[i] = i;
	}
	else {
		for (int i = 0; i < polygon->vertices->len; i++)
			v[i] = (polygon->vertices->len - 1) - i;
	}
	int nv = polygon->vertices->len;
	int count = 2 * nv;
	for (int j = nv - 1; nv > 2; )
	{
		if ((count--) <= 0)
		{
			free(v);
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
	free(v);
	return (TRUE);
}

t_bool	compute_change_of_basis(t_polygon *poly, t_mat4 *p_inv)
{
	const t_vec3	n = get_polygon_normal(poly);
	const t_vec3	up = (t_vec3){0, 0, 1};
	t_vec3			u;
	t_vec3			w;

	u = ft_vec3_cross(n, up);
	if (ft_vec3_len(u) == 0)
		u = (t_vec3){0, 1, 0};
	w = ft_vec3_cross(u, n);
	poly->matrix = (t_mat4)((t_mat4_data){
		u.x, w.x, n.x, 0,
		u.y, w.y, n.y, 0,
		u.z, w.z, n.z, 0,
		0, 0, 0, 1
	});
	return (mat4_inverse(poly->matrix, p_inv));
}

t_bool	triangulate_polygon(t_polygon *polygon)
{
	t_mat4	p_inv;
	int		i;

	if (!compute_change_of_basis(polygon, &p_inv))
		return (FALSE);
	i = -1;
	while (++i < polygon->vertices->len)
		polygon->vertices->vertices[i] = ft_mat4_mulv(p_inv,
			polygon->vertices->vertices[i]);
	ear_clip_polygon(polygon);
	uv_mapping(polygon);
	i = -1;
	while (++i < polygon->vertices->len)
		polygon->vertices->vertices[i] = ft_mat4_mulv(polygon->matrix,
			polygon->vertices->vertices[i]);
	return (TRUE);
}
