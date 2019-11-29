/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/29 22:20:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <maths.h>
#include <stdlib.h>
#include "render.h"
#include "maths/mat4.h"
#include "arrays.h"

#define EPSILON (1e-6)

static float	area(t_4dvertices *vertices, int *filters, int filters_count)
{
	int n = filters_count;
	float A = 0.0f;
	for (int p = n - 1, q = 0; q < n; p = q++) {
		t_vec4 pval = vertices->vertices[filters[p]];
		t_vec4 qval = vertices->vertices[filters[q]];
		A += pval.x * qval.y - qval.x * pval.y;
	}
	return (A * 0.5f);
}

t_bool	inside_triangle(t_vec4 a, t_vec4 b, t_vec4 c, t_vec4 p)
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

static t_bool	snip(t_4dvertices *vertices, int u, int j, int w, int n, int *v)
{
	int		p;
	t_vec4	a, b, c;

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

t_bool	ear_clip2(int *filters, int filters_count, t_4dvertices *vertices, t_faces **faces, int face_type, int face_material)
{
	int		*v;

	if (filters_count < 3
		|| !(v = (int *)malloc(filters_count * sizeof(int))))
		return (FALSE);
	if (area(vertices, filters, filters_count) > 0) {
		for (int i = 0; i < filters_count; i++)
			v[i] = filters[i];
	}
	else {
		for (int i = 0; i < filters_count; i++)
			v[i] = filters[(filters_count - 1) - i];
	}
	int nv = filters_count;
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
		if (snip(vertices, u, j, w, nv, v))
		{
			int s, t;
			t_face face;

			ft_bzero(&face, sizeof(t_face));
			face.normal_type = 1;
			face.has_collision = TRUE;
			face.vertices_index[0] = v[u] + 1;
			face.vertices_index[1] = v[j] + 1;
			face.vertices_index[2] = v[w] + 1;
			face.vertex_set = TRUE;
			face.vertex_index[0] = v[u] + 1;
			face.vertex_index[1] = v[j] + 1;
			face.vertex_index[2] = v[w] + 1;
			face.normals_set = TRUE;
			face.normals_index[0] = v[u] + 1;
			face.normals_index[1] = v[j] + 1;
			face.normals_index[2] = v[w] + 1;
			face.mtl_index = face_material;
			append_faces_array(&*faces, face);
			for (s = j, t = j + 1; t < nv; s++, t++)
				v[s] = v[t];
			nv--;
			count = 2 * nv;
		}	
	}
	free(v);
	return (TRUE);
}

t_bool	compute_change_of_basis(t_vec3 n, t_mat4 *p_inv, t_mat4 *reverse)
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
