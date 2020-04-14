/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree_intersect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:20:28 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:21:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "octree.h"
#include <libft.h>
#include "doom.h"

void			ray_hit_coll_triangle(t_ray *ray, t_renderable *r,
	t_collision *closest_hit, t_collision hit)
{
	if (hit.collide)
	{
		hit = to_world_collision(*ray, hit, r->position, r->rotation,
			r->scale);
		if (hit.dist > 0 && hit.dist < closest_hit->dist)
		{
			*closest_hit = hit;
			closest_hit->renderable = r;
		}
	}
}

void			ray_intersect_octree(t_octree_node *n, t_renderable *r,
	t_ray *ray, t_collision *closest_hit)
{
	int			i;
	t_collision	ahit;
	t_collision hit;

	ahit = ray_hit_aabb(ray->to_local, &n->box.data.aabb);
	if (!ahit.collide)
		return ;
	if (n->faces_index)
	{
		i = -1;
		while (++i < n->faces_index->len)
		{
			if (ray->doom && ray->doom->main_context.type == CTX_NORMAL
				&& !r->faces->values[n->faces_index->values[i]].has_collision)
				continue;
			hit = ray_hit_triangle(ray->to_local, &r->faces->values[
				n->faces_index->values[i]].collidable.data.triangle);
			ray_hit_coll_triangle(ray, r, closest_hit, hit);
		}
	}
	if (!n->childs)
		return ;
	i = -1;
	while (++i < 8)
		ray_intersect_octree(&n->childs[i], r, ray, closest_hit);
}

void			aabb_intersect_octree(t_octree_node *n, t_collide_aabb *aabb,
	void (*fn)(int face, void *param), void *param)
{
	int				i;
	t_collision		ahit;

	ahit = aabb_hit_aabb(aabb, &n->box.data.aabb);
	if (!ahit.collide)
		return ;
	if (n->faces_index)
	{
		i = -1;
		while (++i < n->faces_index->len)
			fn(n->faces_index->values[i], param);
	}
	if (!n->childs)
		return ;
	i = -1;
	while (++i < 8)
		aabb_intersect_octree(&n->childs[i], aabb, fn, param);
}

void			frustum_intersect_octree(t_octree_node *n, t_vec4 *frustum,
	void (*fn)(int face, void *param), void *param)
{
	int				i;

	if (!aabb_vs_frustrum(n->box.data.aabb, frustum))
		return ;
	if (n->faces_index && !n->childs)
	{
		i = -1;
		while (++i < n->faces_index->len)
			fn(n->faces_index->values[i], param);
	}
	if (!n->childs)
		return ;
	i = -1;
	while (++i < 8)
		frustum_intersect_octree(&n->childs[i], frustum, fn, param);
}
