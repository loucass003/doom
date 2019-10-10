/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 00:02:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 00:38:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "render.h"

t_bool		free_renderable(t_renderable **r, t_bool res)
{
	*r = NULL;
	return (res);
}

void		transform_renderable(t_renderable *r)
{
	int		i;

	const	t_mat4 rot = ft_mat4_rotation(r->rotation);
	const	t_mat4 m = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(r->position),
			rot
		),
		ft_mat4_scale(r->scale)
	); 

	i = -1;
	while (++i < r->vertices->len)
		r->pp_vertices[i] = mat4_mulv4(m, r->vertices->vertices[i]);
	i = -1;
	while (++i < r->normals->len)
		r->pp_normals[i] = ft_mat4_mulv(rot, r->normals->vertices[i]);
	i = -1;
	while (++i < r->faces->len)
	{
		t_face *face = &r->faces->values[i];
		face->face_normal = get_triangle_normal(
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]));
	}
}
