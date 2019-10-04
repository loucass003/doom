/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_post_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/03 21:25:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "collision.h"
#include "maths/triangle.h"
#include "render.h"
#include "octree.h"

/* 
void		transfom_obj(t_obj *obj)
{
	int		i;

	const	t_mat4 rot = ft_mat4_rotation(obj->rotation);
	const	t_mat4 m = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(obj->position),
			rot
		),
		ft_mat4_scale(obj->scale)
	); 

	i = -1;
	while (++i < obj->vertices->len)
		obj->pp_vertices[i] = mat4_mulv4(m, obj->vertices->vertices[i]);
	i = -1;
	while (++i < obj->normals->len)
		obj->pp_normals[i] = ft_mat4_mulv(rot, obj->normals->vertices[i]);
	i = -1;
	while (++i < obj->faces->len)
	{
		t_face *face = &obj->faces->values[i];
		face->face_normal = get_triangle_normal(
			vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(obj->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(obj->pp_vertices[face->vertices_index[2] - 1]));
	}
}
 */



t_bool			post_process_obj(t_renderable *r)
{
	int			i;
	t_face		*face;


	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->collidable = compute_collidable(r, i, r->vertices->vertices);
	}
	r->octree = create_octree(r);
	printf("%p %d\n", r->octree, r->faces->len);
	print_octree(r->octree);
	transform_renderable(r);
	
	return (TRUE);
}
