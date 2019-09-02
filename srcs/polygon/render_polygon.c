/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_polygon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/02 16:04:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "maths/vec4.h"
#include "maths/triangle.h"
#include "render.h"

t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	return (r);
}

typedef union ut_color {
	uint32_t color;
	struct argbTag
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
        uint8_t a;
	} argb;
	t_color	color_t;
} ur_color;


void	render_polygon(t_render_context *ctx, t_polygon *poly)
{
	t_mtl		mtl = (t_mtl){
		.texture_map_set = TRUE,
		.texture_map = poly->texture,
		.lightmap = poly->lightmap
	};
	const size_t	len = poly->indices->len / 3;
	size_t			i;

	if (poly->pointed)
	{
		mtl.material_color_set = TRUE;
		mtl.material_color = 0xFFFF0000;
	}

	i = -1;
	while (++i < len)
	{
		float d = ft_vec3_dot(poly->normals[i], ft_vec3_sub(ctx->camera->pos, poly->vertices->vertices[poly->indices->values[i * 3]]));
		if (d <= 0)
			continue;
		// float it = fmax(0, ft_vec3_dot(ft_vec3_inv(poly->normals[i]), ft_vec3_sub((t_vec3){1, 0, 1}, (t_vec3){ 0, 0, 3 })));
		// it = fmax(0, fmin(1, 0.2 + it)) * 255;
		// uint8_t light_color = it;
		process_triangle(ctx, (t_mtl *)&mtl, (t_triangle){
			{.pos = mat43_mulv4(ctx->camera->matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3]])), .tex = poly->uvs[poly->indices->values[i * 3]], .normal = poly->normals[i]},
			{.pos = mat43_mulv4(ctx->camera->matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3 + 1]])), .tex = poly->uvs[poly->indices->values[i * 3 + 1]], .normal =  poly->normals[i]},
			{.pos = mat43_mulv4(ctx->camera->matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3 + 2]])), .tex = poly->uvs[poly->indices->values[i * 3 + 2]], .normal =  poly->normals[i]}
		});
	}
}
