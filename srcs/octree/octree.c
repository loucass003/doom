/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:31:15 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/04 21:18:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "octree.h"
#include "collision.h"
#include "render.h"

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
	box.min = ft_vec3_sub(box.min, (t_vec3){ 1e-2, 1e-2, 1e-2 });
	box.max = ft_vec3_add(box.max, (t_vec3){ 1e-2, 1e-2, 1e-2 });
	printf("min %f %f %f, max %f %f %f\n", box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z);
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
	int		i;
	int		j;
	int		k;
	t_vec3	s;
	t_octree_node *c;

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
					.min = ft_vec3_add(p.min, ft_vec3_mul(s, (t_vec3){ i, j, k })),
					.max = ft_vec3_add(s, ft_vec3_add(p.min, ft_vec3_mul(s, (t_vec3){ i, j, k })))
				};
				printf("min %f %f %f, max %f %f %f\n", c->box.data.aabb.min.x, c->box.data.aabb.min.y, c->box.data.aabb.min.z, c->box.data.aabb.max.x, c->box.data.aabb.max.y, c->box.data.aabb.max.z);
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

t_bool			insert_octree(t_octree_node *n, t_renderable *r, int face)
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
				if (!insert_octree(n, r, n->faces_index->values[i]))
					return (FALSE);
			free(n->faces_index);
			n->faces_index = NULL;
		}
	}
	else
	{
		i = -1;
		while (++i < 8)
			if (triangle_hit_aabb(&r->faces->values[face].collidable.data.triangle, &n->childs[i].box.data.aabb).collide)
				if (!insert_octree(&n->childs[i], r, face))
					return (FALSE);
	}
	return (TRUE);
}

t_octree_node	*create_octree(t_renderable *r)
{
	int				i;
	t_octree_node	*root;

	// if (r->faces->len <= 100)
	// 	return (NULL);
	if (!(root = create_node(get_maximum_box(r))))
		return (NULL);
	i = -1;
	while (++i < r->faces->len)
		if (!insert_octree(root, r, i))
			return (NULL);
	return (root);
}

void			ray_intersect_octree(t_octree_node *n, t_renderable *r, t_ray *ray, t_collision *closest_hit)
{
	int		i;
	t_collision	ahit;

	ahit = ray_hit_aabb(ray->to_local, &n->box.data.aabb);
	if (!ahit.collide)
		return;
	if (n->faces_index)
	{
		int i = -1;
		while (++i < n->faces_index->len)
		{
			t_collision hit = ray_hit_triangle(ray->to_local, &r->faces->values[n->faces_index->values[i]].collidable.data.triangle);
			if (hit.collide)
			{
				t_ray *local_ray = ray->to_local;
				t_vec3 point = ft_vec3_add(local_ray->origin, ft_vec3_mul_s(local_ray->direction, hit.dist));
				point = ft_vec3_mul(point, r->scale);
				point = vec3_rotate(point, r->rotation);
				point = ft_vec3_add(point, r->position);
				float dist = ft_vec3_len(ft_vec3_sub(ray->origin, point));
				hit.dist = dist;
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


void			print_octree(t_octree_node	*n)
{
	int i;
	if (!n || !n->childs)
		return ;
	i = -1;
	while (++i < 8)
	{
		print_octree(&n->childs[i]);
		if (n->childs[i].faces_index)
			printf("%p(%d) --> %p(%d -- %d)\n", n,  0, n->childs + i, i, n->childs[i].faces_index->len);
	}
}
