/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid_renderable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 21:56:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/25 14:56:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "render.h"
#include "ellipsoid.h"

static t_bool		create_ellipsoid_faces(t_renderable *r, t_vec2 res)
{
	t_vec2	i;
	int	first;
	int	second;
	t_face	face;

	i.x = -1;
	while (++i.x < res.x)
	{
		i.y = -1;
		while (++i.y < res.y)
		{
			first = (i.x * (res.y + 1)) + i.y + 1;
			second = first + res.y + 1;
			face = (t_face){ .has_collision = TRUE, .normal_type = 1, .vertices_index = { first, second, first + 1 },
				.normals_index = { first, second, first + 1 },
				.mtl_index = 0 };
			if (!append_faces_array(&r->faces, face))
				return (FALSE);
			face = (t_face){ .has_collision = TRUE, .normal_type = 1, .vertices_index = { second, second + 1, first + 1 },
				.normals_index = { second, second + 1, first + 1 },
				.mtl_index = 0 };
			if (!append_faces_array(&r->faces, face))
				return (FALSE);
		}
	}
	return (TRUE);
}

static t_bool		gen_ellipsoid_mesh(t_renderable *r, t_vec2 res, t_vec3 radius)
{
	t_vec2	i;
	t_vec2	v_theta;
	float	theta;
	float	phi;
	t_vec2	v_phi;

	i.x = -1;
	while (++i.x <= res.x)
	{
		theta = (i.x * M_PI * 2 / res.x);
		v_theta = (t_vec2){ cosf(theta), sinf(theta) };
		i.y = -1;
		while (++i.y <= res.y)
		{
			phi = (i.y * 2 * M_PI / res.y);
			v_phi = (t_vec2){ cosf(phi), sinf(phi) };
			t_vec3 v = ft_vec3_mul(radius, (t_vec3){ v_phi.x * v_theta.x, v_theta.x * v_phi.y, v_theta.y });
			if (!append_4dvertices_array(&r->vertices, vec3_to_4(v)))
				return (FALSE);
			if (!append_3dvertices_array(&r->normals, ft_vec3_norm(v)))
				return (FALSE);
		}
	}
	return (create_ellipsoid_faces(r, res));
}

t_bool	create_ellipsoid(t_renderable *r, int color, t_vec2 res, t_vec3 radius)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_ELLIPSOID;
	if (!(r->of.data.ellipsoid = ft_memalloc(sizeof(t_ellipsoid))))
		return (FALSE);
	r->of.data.ellipsoid->radius = radius;
	if(!(r->vertices = create_4dvertices_array(100)))
		return (free_renderable(&r, FALSE));
	if(!(r->normals = create_3dvertices_array(100)))
		return (free_renderable(&r, FALSE));
	if(!(r->faces = create_faces_array(50)))
		return (free_renderable(&r, FALSE));
	if(!(r->materials = create_mtllist(1)))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, (t_mtl){ 
			.material_color_set = TRUE, .material_color = color }))
		return (free_renderable(&r, FALSE));
	gen_ellipsoid_mesh(r, res, radius);
	if (!(r->pp_vertices = (t_vec4 *)malloc(sizeof(t_vec4) * r->vertices->len)))
		return (FALSE);
	if (!(r->pp_normals = (t_vec3 *)malloc(sizeof(t_vec3) * r->normals->len)))
		return (FALSE);
	r->dirty = TRUE;
	r->fixed = FALSE; 
	r->scale = (t_vec3){ 1, 1, 1 };
	//r->double_faced = TRUE;
	return (TRUE);
}
