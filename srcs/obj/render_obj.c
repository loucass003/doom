/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 15:44:57 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/26 06:04:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

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

static void		update_obj(t_doom *doom, t_obj *obj)
{
	int		i;
	const	t_mat4 m = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(obj->position),
			ft_mat4_rotation(obj->rotation)
		),
		ft_mat4_scale(obj->scale)
	); 

	i = -1;
	while (++i < obj->vertices->len)
		obj->pp_vertices[i] = mat4_mulv4(m, obj->vertices->vertices[i]);
	i = -1;
	while (++i < obj->normals->len)
		obj->pp_normals[i] = obj->normals->vertices[i] /* ft_mat4_mulv(m, obj->normals->vertices[i])*/;
	i = -1;
	while (++i < obj->faces->len)
	{
		t_face *face = &obj->faces->values[i];
		face->face_normal = get_plane_normal(
			vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1]), 
			vec4_to_3(obj->pp_vertices[face->vertices_index[1] - 1]), 
			vec4_to_3(obj->pp_vertices[face->vertices_index[2] - 1]));
	}
	obj->dirty = FALSE;
}

void	render_obj(t_doom *doom, t_obj *obj)
{
	int		i;
	t_face	*face;

	if (obj->dirty)
		update_obj(doom, obj);
	i = -1;
	while (++i < obj->faces->len)
	{
		face = &obj->faces->values[i];
		if (!face->mtl->texture_map_set && !face->mtl->material_color_set)
		{
			face->mtl->material_color_set = TRUE;
			face->mtl->material_color = 0x555555;
		}
	 	float d = ft_vec3_dot(face->face_normal, ft_vec3_sub(doom->player.pos, vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1])));
		if (d <= 0)
			continue;
		process_triangle(doom, face->mtl, (t_triangle){
			{ .pos = mat43_mulv4(doom->player.matrix, obj->pp_vertices[face->vertices_index[0] - 1]), .tex = obj->vertex->vertices[face->vertex_index[0] - 1], .normal = obj->pp_normals[face->normals_index[0] - 1]},
			{ .pos = mat43_mulv4(doom->player.matrix, obj->pp_vertices[face->vertices_index[1] - 1]), .tex = obj->vertex->vertices[face->vertex_index[1] - 1], .normal = obj->pp_normals[face->normals_index[1] - 1]},
			{ .pos = mat43_mulv4(doom->player.matrix, obj->pp_vertices[face->vertices_index[2] - 1]), .tex = obj->vertex->vertices[face->vertex_index[2] - 1], .normal = obj->pp_normals[face->normals_index[2] - 1]}
		});
	}
}
