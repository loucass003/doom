/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 12:23:43 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <maths.h>
#include <stdlib.h>
#include "render.h"
#include "maths/mat4.h"
#include "arrays.h"

#define EPSILON (1e-6)

static float	area(t_4dvertices *vertices, int *filters, int filters_count)
{
	float	a;
	int		p;
	int		q;
	t_vec4	pval;
	t_vec4	qval;

	a = 0.0f;
	p = filters_count - 1;
	q = 0;
	while (q < filters_count)
	{
		pval = vertices->vertices[filters[p]];
		qval = vertices->vertices[filters[q]];
		a += pval.x * qval.y - qval.x * pval.y;
		p = q++;
	}
	return (a * 0.5f);
}

t_bool			inside_triangle(t_vec4 a, t_vec4 b, t_vec4 c, t_vec4 p)
{
	float	i;
	float	j;
	float	k;

	i = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
	j = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
	k = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);
	return (i >= 0 && j >= 0 && k >= 0);
}

static t_bool	snip(t_4dvertices *vertices, int u, int j, int w, int n, int *v)
{
	int		p;
	t_vec4	a;
	t_vec4	b;
	t_vec4	c;
	float	t;

	a = vertices->vertices[v[u]];
	b = vertices->vertices[v[j]];
	c = vertices->vertices[v[w]];
	t = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
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

t_bool			ear_clip2(int *filters, int filters_count, t_4dvertices *vertices,
	t_faces **faces, int normal_type, int face_material, int room_index)
{
	int		*indices;
	int		nv;
	int		count;
	int		v;

	if (filters_count < 3
		|| !(indices = (int *)malloc(filters_count * sizeof(int))))
		return (FALSE);
	if (area(vertices, filters, filters_count) > 0) {
		for (int i = 0; i < filters_count; i++)
			indices[i] = filters[i];
	}
	else {
		for (int i = 0; i < filters_count; i++)
			indices[i] = filters[(filters_count - 1) - i];
	}
	nv = filters_count;
	count = 2 * nv;
	v = nv - 1;
	while (nv > 2)
	{
		if (count-- <= 0)
		{
			free(indices);
			return (TRUE);
		}
		int u;
		u = v;
		if (nv <= u)
			u = 0;
		v = u + 1;
		if (nv <= v)
			v = 0;
		int w;
		w = v + 1;
		if (nv <= w)
			w = 0;
		if (snip(vertices, u, v, w, nv, indices))
		{
			int	s;
			int	t;
			t_face	face;

			ft_bzero(&face, sizeof(t_face));
			face.hidden = 0;
			face.normal_type = normal_type;
			face.double_sided = normal_type == 2;
			face.has_collision = TRUE;
			face.vertices_index[0] = indices[u] + 1;
			face.vertices_index[1] = indices[v] + 1;
			face.vertices_index[2] = indices[w] + 1;
			face.vertex_set = TRUE;
			face.vertex_index[0] = indices[u] + 1;
			face.vertex_index[1] = indices[v] + 1;
			face.vertex_index[2] = indices[w] + 1;
			face.normals_set = TRUE;
			face.normals_index[0] = indices[u] + 1;
			face.normals_index[1] = indices[v] + 1;
			face.normals_index[2] = indices[w] + 1;
			face.mtl_index = face_material;
			face.wall_index = -1;
			face.room_index = room_index;
			append_faces_array(&*faces, face);
			s = v;
			t = v + 1;
			while (t < nv)
				indices[s++] = indices[t++];
			nv--;
			count = 2 * nv;
		}
	}
	free(indices);
	return (TRUE);
}

t_bool			compute_change_of_basis(t_vec3 n, t_mat4 *p_inv,
		t_mat4 *reverse)
{
	const t_vec3	up = (t_vec3){0, 0, 1};
	t_vec3			u;
	t_vec3			w;

	u = ft_vec3_cross(n, up);
	if (ft_vec3_len(u) == 0)
		u = (t_vec3){0, 1, 0};
	w = ft_vec3_cross(u, n);
	reverse->d = (t_mat4_data){
		u.x, w.x, n.x, 0,
		u.y, w.y, n.y, 0,
		u.z, w.z, n.z, 0,
		0, 0, 0, 1
	};
	return (mat4_inverse(*reverse, p_inv));
}
