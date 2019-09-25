/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:31:15 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/25 15:02:55 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "octree.h"
#include "collision.h"
#include "render.h"

t_collidable	get_maximum_box(t_renderable *r)
{
	t_collidable	col;
	t_collide_aabb	box;
	t_vec4			vert;
	int				i;

	col.type = COLLIDE_AABB;
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
	col.data.aabb = box;
	printf("min %f %f %f, max %f %f %f\n", box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z);
	return (col);
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
			}
		}
	}
}

t_bool			subdivide(t_renderable *r, t_octree_node *n)
{
	int				i;
	int				j;
	t_octree_node	*child;

	if (!(n->childs = ft_memalloc(sizeof(t_octree_node) * 8)))
		return (FALSE);
	compute_boxes(n->box.data.aabb, n);
	j = -1;
	while (++j < 8)
	{
		child = n->childs + j;
		child->parent = n;
		if (!(child->faces_index = create_ints_array(n->faces_index->len / 5 + 1)))
			return (FALSE);
		i = -1;
		while (++i < n->faces_index->len)
		{
			if (triangle_hit_aabb(&r->faces->values[i].collidable.data.triangle, &child->box.data.aabb).collide)
				append_ints_array(&child->faces_index, i);
		}
		if (child->faces_index->len > 100)
		{
			if (!subdivide(r, child))
			{
			//	n->childs = NULL;
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

t_octree_node	*create_octree(t_renderable *r)
{
	int				i;
	t_octree_node	*root;

	if (!(root = create_node(get_maximum_box(r))))
		return (NULL);
	if (r->faces->len <= 100)
		return (NULL);
	if (!(root->faces_index = create_ints_array(r->faces->len)))
		return (NULL);
	i = -1;
	while (++i < r->faces->len)
		append_ints_array(&root->faces_index, i);
	if (!subdivide(r, root))
		return (NULL);
	return (root);
}

void			print_octree(t_octree_node	*n)
{
	int i;
	if (!n || !n->childs)
		return ;
	i = -1;
	while (++i < 8)
	{
		if (n->childs + i != NULL)
		{
			print_octree(&n->childs[i]);
			printf("%p(%d) --> %p(%d -- %d)\n", n,  n->faces_index->len, n->childs + i, i, n->childs[i].faces_index->len);
		}
	}
}
