/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 15:44:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/19 01:06:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "maths/vec4.h"
#include "maths/triangle.h"
#include "obj.h"
#include "render.h"
#include "doom.h"


static t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	//r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}

// static void		update_obj(t_obj *obj)
// {
// 	int		i;

// 	const	t_mat4 rot = ft_mat4_rotation(obj->rotation);
// 	const	t_mat4 m = ft_mat4_mul(
// 		ft_mat4_mul(
// 			ft_mat4_translation(obj->position),
// 			rot
// 		),
// 		ft_mat4_scale(obj->scale)
// 	); 

// 	i = -1;
// 	while (++i < obj->vertices->len)
// 		obj->pp_vertices[i] = mat4_mulv4(m, obj->vertices->vertices[i]);
// 	i = -1;
// 	while (++i < obj->normals->len)
// 		obj->pp_normals[i] = ft_mat4_mulv(rot, obj->normals->vertices[i]);
// 	i = -1;
// 	while (++i < obj->faces->len)
// 	{
// 		t_face *face = &obj->faces->values[i];
// 		face->face_normal = get_triangle_normal(
// 			vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1]), 
// 			vec4_to_3(obj->pp_vertices[face->vertices_index[1] - 1]), 
// 			vec4_to_3(obj->pp_vertices[face->vertices_index[2] - 1]));
// 	}
// 	obj->dirty = FALSE;
// }

float	get_light_intensity(t_render_context *ctx, t_vec3 normal, t_vec4 point)
{
	int		i;
	float	sum;
	int		valid;
	t_light	*light;
	
	i = -1;
	sum = 0;
	valid = 0;
	while (++i < ctx->doom->lights->len)
	{
		light = &ctx->doom->lights->values[i];
		// float d = ft_vec3_dot(normal, ft_vec3_sub(light->position, vec4_to_3(point)));
		// if (d > 5)
		// 	continue;
		valid++;
		light->rotation.y -=light->rotation.y;
		sum += ft_max(AMBIANT_LIGHT, fmin(1, fmax(0, ft_vec3_dot(normal, ft_vec3_sub(light->rotation, light->position)))) * 255);
	}
	if (valid == 0)
		return 25;
	return sum / (float)valid;
}

void	render_obj(t_render_context *ctx, t_obj *obj)
{
	/* int		i;
	t_face	*face;

	if (obj->dirty)
	{
		transfom_obj(obj);
		obj->dirty = FALSE;
	}
	i = -1;
	while (++i < obj->faces->len)
	{
		face = &obj->faces->values[i];
		if (!face->mtl->texture_map_set && !face->mtl->material_color_set)
		{
			face->mtl->material_color_set = TRUE;
			face->mtl->material_color = 0xFF555555;
		}
	 	float d = ft_vec3_dot(face->face_normal, ft_vec3_sub(ctx->camera->pos, vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1])));
		if (d <= 0)
			continue;
		
		t_vec4 v0 = mat43_mulv4(ctx->camera->matrix, obj->pp_vertices[face->vertices_index[0] - 1]);
		t_vec4 v1 = mat43_mulv4(ctx->camera->matrix, obj->pp_vertices[face->vertices_index[1] - 1]);
		t_vec4 v2 = mat43_mulv4(ctx->camera->matrix, obj->pp_vertices[face->vertices_index[2] - 1]);

		float it0 = get_light_intensity(ctx, obj->pp_normals[face->normals_index[0] - 1], obj->pp_vertices[face->vertices_index[0] - 1]);
		float it1 = get_light_intensity(ctx, obj->pp_normals[face->normals_index[1] - 1], obj->pp_vertices[face->vertices_index[1] - 1]);
		float it2 = get_light_intensity(ctx, obj->pp_normals[face->normals_index[2] - 1], obj->pp_vertices[face->vertices_index[2] - 1]);
		process_triangle(ctx, face->mtl, (t_triangle) {
			{ .pos = v0, .tex = obj->vertex->vertices[face->vertex_index[0] - 1], .normal = obj->pp_normals[face->normals_index[0] - 1], .light_color = it0 },
			{ .pos = v1, .tex = obj->vertex->vertices[face->vertex_index[1] - 1], .normal = obj->pp_normals[face->normals_index[1] - 1], .light_color = it1 },
			{ .pos = v2, .tex = obj->vertex->vertices[face->vertex_index[2] - 1], .normal = obj->pp_normals[face->normals_index[2] - 1], .light_color = it2 }
		});
	} */
}
