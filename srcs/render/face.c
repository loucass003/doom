/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   face.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 23:19:27 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 01:17:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

t_bool	prepare_face(t_face *face, t_mtl *mtl, t_renderable *r,
	t_render_context *ctx)
{
	if (r->no_collision)
		face->has_collision = FALSE;
	if (r->double_faced)
		face->double_sided = TRUE;
	if (r->wireframe)
		mtl->wireframe = TRUE;
	if (r->wireframe_color == 0)
		r->wireframe_color = 0xFF555555;
	if (mtl->wireframe || (!mtl->texture_map_set && !mtl->material_color_set))
	{
		mtl->material_color_set = TRUE;
		mtl->material_color = mtl->wireframe ? r->wireframe_color : 0xFF555555;
	}
	if (!face->double_sided
		&& ft_vec3_dot(face->face_normal, ft_vec3_sub(ctx->camera->pos,
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]))) <= 0)
		return (FALSE);
	face->rendered = TRUE;
	return (TRUE);
}

void	render_face(int face_index, void *p)
{
	t_face_data			*face_data;
	t_renderable		*r;
	t_render_context	*ctx;
	t_face				*face;
	t_mtl				*mtl;

	face_data = p;
	r = face_data->r;
	ctx = face_data->ctx;
	face = &r->faces->values[face_index];
	if (face->hidden || face->rendered)
		return ;
	mtl = &r->materials->values[face->mtl_index];
	if (!prepare_face(face, mtl, r, ctx))
		return ;
	prepare_triangle(face_data, face, mtl);
}
