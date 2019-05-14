/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_polygon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:31:56 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/14 13:24:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_polygon	create_wall_polygon(t_line	line, float floor, float ceil)
{
	t_polygon	poly;

	poly = create_polygon(create_3dvertices_array(4), P_WALL);
	append_3dvertices_array(&poly.vertices,
		(t_vec3){ line.a.x, floor, line.a.y });
	append_3dvertices_array(&poly.vertices,
		(t_vec3){ line.b.x, floor, line.b.y });
	append_3dvertices_array(&poly.vertices,
		(t_vec3){ line.b.x, ceil, line.b.y });
	append_3dvertices_array(&poly.vertices,
		(t_vec3){ line.a.x, ceil, line.a.y });
	return (poly);
}

t_polygon	create_polygon(t_3dvertices *vertices, t_polygon_type type)
{
	t_polygon	poly;
	poly.type = type;
	poly.vertices = vertices;
	poly.proj_vertices = NULL;
	poly.proj_vertices_buf = NULL;
	return (poly);
}
