/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 16:53:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/06 13:53:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCTREE_H
# define OCTREE_H

#include "collision.h"
#include "arrays.h"
#include "render.h"

typedef struct s_octree_node	t_octree_node;

struct							s_octree_node
{
	t_octree_node	*parent;
//	int				childs_count;
	t_octree_node	*childs;
	t_collidable	box;
	int				renderable_index;
	t_ints			*faces_index;
};

t_octree_node	*create_octree(t_renderable *r);

#endif
