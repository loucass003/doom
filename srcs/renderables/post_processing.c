/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/21 15:10:04 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "collision.h"
#include "maths/triangle.h"
#include "render.h"
#include "octree.h"

t_bool			post_process_renderable(t_doom *doom, t_renderable *r,
	t_bool octree, t_bool replace)
{
	if (!r->pp_vertices || replace)
	{
		if (replace)
			free(r->pp_vertices);
		if (!(r->pp_vertices = malloc(sizeof(t_vec4) * r->vertices->len)))
			return (FALSE);
	}
	if (!r->pp_normals || replace)
	{
		if (replace)
			free(r->pp_normals);
		if (!(r->pp_normals = malloc(sizeof(t_vec3) * r->normals->len)))
			return (FALSE);
	}
	compute_collidables(r);
	if (octree)
	{
		r->octree = NULL;
		r->octree = create_octree(doom, r);
	}
	transform_renderable(r);
	return (TRUE);
}
