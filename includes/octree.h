/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 16:53:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:23:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCTREE_H
# define OCTREE_H

# include "collision.h"
# include "arrays.h"
# include "render.h"
# include "doom.h"

typedef struct s_octree_node	t_octree_node;

struct							s_octree_node
{
	t_octree_node				*parent;
	t_octree_node				*childs;
	t_collidable				box;
	t_ints						*faces_index;
};

t_bool							insert_octree(t_octree_node *n, t_doom *doom,
									t_renderable *r, int face);
t_octree_node					*create_octree(t_doom *doom, t_renderable *r);
void							ray_intersect_octree(t_octree_node *n,
									t_renderable *r, t_ray *ray,
									t_collision *closest_hit);
void							aabb_intersect_octree(t_octree_node *n,
									t_collide_aabb *aabb,
									void (*fn)(int face, void *param),
									void *param);
void							print_octree(t_octree_node	*n);
void							frustum_intersect_octree(t_octree_node *n,
									t_vec4 *frustum,
									void (*fn)(int face, void *param),
									void *param);
void							free_octree_nodes(t_octree_node *n);
void							free_octree(t_octree_node **n);
void							collide_with_octree(t_renderable *r,
	t_entity *entity, t_octree_node *octree, t_collide_aabb area);

#endif
