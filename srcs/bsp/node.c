/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:26:20 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/22 23:16:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	print_node(t_node *n)
{
	if (!n)
		return ;
	if (n->front)
		printf("N%p ==> N%p\n", n, n->front);
	if (n->back)
		printf("N%p --> N%p\n", n, n->back);
	if (n->type == N_NODE)
	{
		printf("N%p(%s - %.3f, %.3f - %.3f, %.3f)\n", n,
			"NODE",
			n->partition.a.x,
			n->partition.a.y,
			n->partition.b.x,
			n->partition.b.y);
	}
	else
	{
		printf("N%p[LEAF]\n", n);
	}
	print_node(n->front);
	print_node(n->back);
}

t_node		*create_node(t_polygons *polygons)
{
	t_node	*node;
	
	if (!(node = (t_node *)ft_memalloc(sizeof(t_node))))
		return (NULL);
	node->polygons = polygons;
	if (get_partition(polygons, &node->partition))
		node->type = N_NODE;
	else
		node->type = N_LEAF;
	return (node);
}
