/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:22:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:22:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "octree.h"
#include <libft.h>
#include "doom.h"

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
