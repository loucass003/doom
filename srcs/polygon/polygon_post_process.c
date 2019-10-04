/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_post_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/04 03:21:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "polygon.h"
#include "maths/triangle.h"
#include "doom.h"
#include "octree.h"
#include "render.h"


t_bool			post_process_polygon(t_renderable *r)
{
	int				i;
	t_face			*face;
	
	if (!r->pp_vertices && !(r->pp_vertices = malloc(sizeof(t_vec4) * r->vertices->len)))
		return (FALSE);
	if (!r->pp_normals && !(r->pp_normals = malloc(sizeof(t_vec3) * r->normals->len)))
		return (FALSE);
	if (!triangulate_polygon(r))
		return (FALSE);
 	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->collidable = compute_collidable(r, i, r->vertices->vertices);
	}
	transform_renderable(r);
	r->octree = create_octree(r);
	print_octree(r->octree);
	return (TRUE);
}
