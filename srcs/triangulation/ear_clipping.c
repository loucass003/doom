/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ear_clipping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 15:51:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 19:35:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <maths.h>
#include <stdlib.h>
#include "render.h"
#include "maths/mat4.h"
#include "arrays.h"

t_bool	snip(t_4dvertices *vertices, int uvw[3], int n, int *indices)
{
	int		p;
	t_vec4	a;
	t_vec4	b;
	t_vec4	c;
	float	t;

	a = vertices->vertices[indices[uvw[0]]];
	b = vertices->vertices[indices[uvw[1]]];
	c = vertices->vertices[indices[uvw[2]]];
	t = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
	if (EPSILON > t)
		return (FALSE);
	p = -1;
	while (++p < n)
	{
		if (p == uvw[0] || p == uvw[1] || p == uvw[2])
			continue;
		if (inside_triangle(a, b, c, vertices->vertices[indices[p]]))
			return (FALSE);
	}
	return (TRUE);
}

void			prepare_indices(int *filters, int filters_count,
	t_4dvertices *vertices, int *indices)
{
	int		i;

	i = -1;
	if (area(vertices, filters, filters_count) > 0)
		while (++i < filters_count)
			indices[i] = filters[i];
	else
		while (++i < filters_count)
			indices[i] = filters[(filters_count - 1) - i];
}

void			append_face(t_triangulate *t, int *indices, int uvw[3],
	t_faces **faces)
{
	t_face	face;

	ft_bzero(&face, sizeof(t_face));
	face.hidden = 0;
	face.normal_type = t->normal_type;
	face.double_sided = t->normal_type == 2;
	face.has_collision = TRUE;
	face.vertices_index[0] = indices[uvw[0]] + 1;
	face.vertices_index[1] = indices[uvw[1]] + 1;
	face.vertices_index[2] = indices[uvw[2]] + 1;
	face.vertex_set = TRUE;
	face.vertex_index[0] = indices[uvw[0]] + 1;
	face.vertex_index[1] = indices[uvw[1]] + 1;
	face.vertex_index[2] = indices[uvw[2]] + 1;
	face.normals_set = TRUE;
	face.normals_index[0] = indices[uvw[0]] + 1;
	face.normals_index[1] = indices[uvw[1]] + 1;
	face.normals_index[2] = indices[uvw[2]] + 1;
	face.mtl_index = t->mtl;
	face.wall_index = -1;
	face.room_index = t->room_index;
	append_faces_array(&*faces, face);
}

t_bool			process(t_triangulate *t, t_faces **faces, int *indices,
	int *var[3])
{
	int		uw[2];
	int		i;
	int		j;

	if ((*var[1])-- <= 0)
		return (FALSE);
	uw[0] = *var[2];
	if (*var[0] <= uw[0])
		uw[0] = 0;
	*var[2] = uw[0] + 1;
	if (*var[0] <= *var[2])
		*var[2] = 0;
	uw[1] = *var[2] + 1;
	if (*var[0] <= uw[1])
		uw[1] = 0;
	if (snip(t->vertices, (int[3]){uw[0], *var[2], uw[1]}, *var[0], indices))
	{
		append_face(t, indices, (int[3]){uw[0], *var[2], uw[1]}, faces);
		i = *var[2];
		j = *var[2] + 1;
		while (j < *var[0])
			indices[i++] = indices[j++];
		*var[1] = 2 * --(*var[0]);
	}
	return (TRUE);
}

t_bool			ear_clip2(t_triangulate *t, t_faces **faces)
{
	int		*indices;
	int		nv;
	int		count;
	int		v;

	if (t->filter_len < 3
		|| !(indices = (int *)malloc(t->filter_len * sizeof(int))))
		return (FALSE);
	prepare_indices(t->filter, t->filter_len, t->vertices, indices);
	nv = t->filter_len;
	count = 2 * nv;
	v = nv - 1;
	while (nv > 2)
		if (!process(t, faces, indices, (int*[3]){&nv, &count, &v}))
			return (free_ret(indices, TRUE));
	return (free_ret(indices, TRUE));
}
