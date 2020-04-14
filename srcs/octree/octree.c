/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:31:15 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:22:58 by llelievr         ###   ########.fr       */
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

void			print_octree(t_octree_node *n)
{
	int	i;

	if (!n || !n->childs)
		return ;
	i = -1;
	while (++i < 8)
		print_octree(&n->childs[i]);
}
