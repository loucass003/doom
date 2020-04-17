/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_triangle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 23:21:18 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 23:25:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doom.h"

static t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	return (r);
}

void			prepare_points(t_vec4 v[3], t_vec2 vertex[3],
	t_face_data *face_data, t_face *face)
{
	t_renderable		*r;
	t_render_context	*ctx;

	r = face_data->r;
	ctx = face_data->ctx;
	vertex[0] = (t_vec2){ 0, 0 };
	vertex[1] = (t_vec2){ 0, 0 };
	vertex[2] = (t_vec2){ 0, 0 };
	v[0] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[0] - 1]));
	v[1] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[1] - 1]));
	v[2] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[2] - 1]));
	if (r->vertex)
	{
		vertex[0] = r->vertex->vertices[face->vertex_index[0] - 1];
		vertex[1] = r->vertex->vertices[face->vertex_index[1] - 1];
		vertex[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	}
}

void			prepare_triangle(t_face_data *fd, t_face *face, t_mtl *mtl)
{
	t_vec4	v[3];
	float	it[3];
	t_vec2	vertex[3];

	prepare_points(v, vertex, fd, face);
	prepare_points_light(it, face, fd);
	process_triangle(fd->ctx, mtl, (t_triangle) {
		{ .pos = v[0], .tex = vertex[0], .light_color = it[0],
			.normal = fd->r->pp_normals[face->normals_index[0] - 1] },
		{ .pos = v[1], .tex = vertex[1], .light_color = it[1],
			.normal = fd->r->pp_normals[face->normals_index[1] - 1] },
		{ .pos = v[2], .tex = vertex[2], .light_color = it[2],
			.normal = fd->r->pp_normals[face->normals_index[2] - 1] }
	});
}
