/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/02 13:18:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "collision.h"
#include "maths/triangle.h"
#include "render.h"
#include "octree.h"

t_bool			post_process_renderable(t_doom *doom, t_renderable *r, t_bool octree)
{
	int			i;
	t_face		*face;

	if (!r->pp_vertices && !(r->pp_vertices = malloc(sizeof(t_vec4) * r->vertices->len)))
		return (FALSE);
	if (!r->pp_normals && !(r->pp_normals = malloc(sizeof(t_vec3) * r->normals->len)))
		return (FALSE);
	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->collidable = face_collidable(r, i, r->vertices->vertices);
	}
	if (octree)
	{
		r->octree = create_octree(doom, r);
		printf("%p %d\n", r->octree, r->faces->len);
		print_octree(r->octree);
	}
	transform_renderable(r);
	return (TRUE);
}