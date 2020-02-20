/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:31:15 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:55:26 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "octree.h"
#include "collision.h"
#include "render.h"
#include "arrays.h"

t_collidable	get_maximum_box(t_renderable *r)
{
	t_collide_aabb	box;
	t_vec4			vert;
	int				i;

	box.max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	box.min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	i = -1;
	while (++i < r->vertices->len)
	{
		vert = r->vertices->vertices[i];
		box.min.x = fmin(box.min.x, vert.x);
		box.min.y = fmin(box.min.y, vert.y);
		box.min.z = fmin(box.min.z, vert.z);
		box.max.x = fmax(box.max.x, vert.x);
		box.max.y = fmax(box.max.y, vert.y);
		box.max.z = fmax(box.max.z, vert.z);
	}
	return ((t_collidable){ .type = COLLIDE_AABB, .data = { .aabb = box } });
}

t_octree_node	*create_node(t_collidable box)
{
	t_octree_node	*node;

	if (!(node = ft_memalloc(sizeof(t_octree_node))))
		return (NULL);
	node->box = box;
	return (node);
}

void			compute_boxes(t_collide_aabb p, t_octree_node *n)
{
	int				i;
	int				j;
	int				k;
	t_vec3			s;
	t_octree_node 	*c;

	i = -1;
	while (++i < 2 && !!(j = -1))
	{
		while (++j < 2 && !!(k = -1))
		{
			while (++k < 2)
			{
				s = ft_vec3_mul_s(ft_vec3_sub(p.max, p.min), 0.5);
				c = &n->childs[i + 2 * (j + 2 * k)];
				c->box.type = COLLIDE_AABB;
				c->box.data.aabb = (t_collide_aabb){
					.min = ft_vec3_add(p.min, ft_vec3_mul(s, (t_vec3)
						{ i, j, k })),
					.max = ft_vec3_add(p.min, ft_vec3_mul(s, (t_vec3)
						{ i + 1, j + 1, k + 1 }))
				};
			}
		}
	}
}

t_bool			subdivide(t_octree_node *n)
{
	if (!n || !(n->childs = ft_memalloc(sizeof(t_octree_node) * 8)))
		return (FALSE);
	compute_boxes(n->box.data.aabb, n);
	return (TRUE);
}

t_bool			insert_octree(t_octree_node *n, t_doom *doom, t_renderable *r,
	int face)
{
	int i;

	if (!n->childs)
	{
		if (!n->faces_index && !(n->faces_index = create_ints_array(50)))
			return (FALSE);
		if (!append_ints_array(&n->faces_index, face))
			return (FALSE);
		if (n->faces_index->len > 100)
		{
			if (!subdivide(n))
				return (FALSE);
			i = -1;
			while (++i < n->faces_index->len)
				if (!insert_octree(n, doom, r, n->faces_index->values[i]))
					return (FALSE);
			free(n->faces_index);
			n->faces_index = NULL;
		}
	}
	else
	{
		i = -1;
		while (++i < 8)
			if (triangle_hit_aabb(&r->faces->values[face]
				.collidable.data.triangle, &n->childs[i].box.data.aabb).collide)
				if (!insert_octree(&n->childs[i], doom, r, face))
					return (FALSE);
	}
	return (TRUE);
}

t_octree_node	*create_octree(t_doom *doom, t_renderable *r)
{
	int				i;
	t_octree_node	*root;

	if (!(root = create_node(get_maximum_box(r))))
		return (NULL);
	i = -1;
	while (++i < r->faces->len)
		if (!insert_octree(root, doom, r, i))
			return (NULL);
	return (root);
}

void			ray_intersect_octree(t_octree_node *n, t_renderable *r,
	t_ray *ray, t_collision *closest_hit)
{
	int			i;
	t_collision	ahit;

	ahit = ray_hit_aabb(ray->to_local, &n->box.data.aabb);
	if (!ahit.collide)
		return;
	if (n->faces_index)
	{
		i = -1;
		while (++i < n->faces_index->len)
		{
			if (ray->doom
				&& ray->doom->main_context.type == CTX_NORMAL
				&& !r->faces->values[n->faces_index->values[i]].has_collision)
				continue;
			t_collision hit;
			hit = ray_hit_triangle(ray->to_local, &r->faces->values[
				n->faces_index->values[i]].collidable.data.triangle);
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

void			print_octree(t_octree_node	*n)
{
	int i;
	if (!n || !n->childs)
		return ;
	i = -1;
	while (++i < 8)
		print_octree(&n->childs[i]);
}

void			free_octree_nodes(t_octree_node *n)
{
	int		i;

	ft_memdel((void **)&n->faces_index);
	if (!n->childs)
		return ;
	i = -1;
	while (++i < 8)
		free_octree_nodes(n->childs + i);
	free(n->childs);
}

void			free_octree(t_octree_node **n)
{
	free_octree_nodes(*n);
	ft_memdel((void **)n);
}
