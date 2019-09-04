/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 16:53:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/04 18:07:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCTREE_H
# define OCTREE_H

#include "collision.h"
#include "arrays.h"

typedef struct s_octree_node	t_octree_node;

struct							s_octree_node
{
	t_octree_node	*parent;
	int				childs_count;
	t_octree_node	*childs;
	t_collidable	box;
	t_ptrs			*elements;
};



#endif
