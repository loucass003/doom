/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_polygon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:31:56 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 22:37:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_polygon	*create_wall_polygon(t_line	line, float floor, float ceil)
{
	t_polygon	*poly;

	if (!(poly = create_polygon(create_arraylist(4), P_WALL)))
		return (NULL);
	arraylist_appendm(&poly->vertices,
		&(t_vec3){ line.a.x, floor, line.a.y }, sizeof(t_vec3));
	arraylist_appendm(&poly->vertices,
		&(t_vec3){ line.b.x, floor, line.b.y }, sizeof(t_vec3));
	arraylist_appendm(&poly->vertices,
		&(t_vec3){ line.b.x, ceil, line.b.y }, sizeof(t_vec3));
	arraylist_appendm(&poly->vertices,
		&(t_vec3){ line.a.x, ceil, line.a.y }, sizeof(t_vec3));
	return (poly);
}

t_polygon	*create_polygon(t_arraylist *vertices, t_polygon_type type)
{
	t_polygon	*poly;

	if (!vertices || !(poly = (t_polygon *)malloc(sizeof(t_polygon))))
	{
		if (vertices)
			;//TODO: free vertices
		return (NULL);
	}
	poly->type = type;
	poly->vertices = vertices;
	poly->proj_vertices = NULL;
	return (poly);
}
