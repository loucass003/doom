/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bsp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:42:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/02 00:36:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	free_polygon(void *e)
{
	t_polygon	*poly = (t_polygon *)e;
	if (poly->vertices)
		arraylist_clear(&poly->vertices, free);
	if (poly->proj_vertices)
		free(poly->proj_vertices);
	free(poly);
}

void	free_node(t_node *n)
{
	if (n->front)
		free_node(n->front);
	if (n->back)
		free_node(n->back);
	if (n->polygons)
		arraylist_clear(&n->polygons, free_polygon);
	free(n);
}
