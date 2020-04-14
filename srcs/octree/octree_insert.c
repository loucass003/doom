/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree_insert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:22:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:22:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "octree.h"
#include <libft.h>
#include "doom.h"

void			compute_boxes(t_collide_aabb p, t_octree_node *n)
{
	int				i;
	int				j;
	int				k;
	t_vec3			s;
	t_octree_node	*c;

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

t_bool			insert_face_index(t_octree_node *n, int face)
{
	if (!n->faces_index && !(n->faces_index = create_ints_array(50)))
		return (FALSE);
	if (!append_ints_array(&n->faces_index, face))
		return (FALSE);
	return (TRUE);
}

t_bool			insert_octree(t_octree_node *n, t_doom *doom, t_renderable *r,
	int face)
{
	int i;

	i = -1;
	if (!n->childs)
	{
		if (insert_face_index(n, face) == FALSE)
			return (FALSE);
		if (n->faces_index->len > 100)
		{
			if (!subdivide(n))
				return (FALSE);
			while (++i < n->faces_index->len)
				if (!insert_octree(n, doom, r, n->faces_index->values[i]))
					return (FALSE);
			free(n->faces_index);
			n->faces_index = NULL;
		}
	}
	else
		while (++i < 8)
			if (triangle_hit_aabb(&r->faces->values[face].collidable.data
				.triangle, &n->childs[i].box.data.aabb).collide)
				if (!insert_octree(&n->childs[i], doom, r, face))
					return (FALSE);
	return (TRUE);
}
