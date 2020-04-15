/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:23:58 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 17:16:57 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "render.h"
#include "editor.h"
#include "door.h"
#include "maths/mat4.h"

static void		transform_vertices(t_renderable *r, t_mat4 m, t_mat4 m2)
{
	int		i;
	t_mat4	a;

	i = -1;
	while (++i < r->vertices->len)
	{
		if (ints_indexof(r->of.data.door->door_1_indexes, i + 1) != -1)
			a = m;
		else if (ints_indexof(r->of.data.door->door_2_indexes, i + 1) != -1)
			a = m2;
		else
			a = r->matrix;
		r->pp_vertices[i] = mat4_mulv4(a, r->vertices->vertices[i]);
	}
}

static void		transform_normals(t_renderable *r, t_mat4 rot)
{
	int		i;

	i = -1;
	while (++i < r->normals->len)
		r->pp_normals[i] = ft_mat4_mulv(rot, r->normals->vertices[i]);
}

static void		update_faces(t_renderable *r)
{
	int		i;
	t_face	*face;
	t_vec3	n;

	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		n = get_triangle_normal(
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]));
		if (face->normal_type == 0)
			face->face_normal = ft_vec3_inv(n);
		else
			face->face_normal = n;
	}
}

void			transform_renderable_door(t_renderable *r)
{
	t_mat4			m;
	t_mat4			m2;
	const t_mat4	rot = ft_mat4_rotation(r->rotation);

	r->matrix = ft_mat4_mul(ft_mat4_mul(ft_mat4_translation(r->position), rot),
		ft_mat4_scale(r->scale));
	m = ft_mat4_mul(r->matrix, ft_mat4_translation((t_vec3){
		r->of.data.door->open_value * 0.6, 0, 0 }));
	m2 = ft_mat4_mul(r->matrix, ft_mat4_translation((t_vec3){
		-r->of.data.door->open_value * 0.6, 0, 0 }));
	transform_vertices(r, m, m2);
	transform_normals(r, rot);
	update_faces(r);
	compute_collidables(r);
}
