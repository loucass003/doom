/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_polygon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:31:56 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/30 04:56:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "polygon.h"
#include "arrays.h"
#include "maths/line.h"

t_polygon	create_wall_polygon(SDL_Surface *texture, t_line line, float floor, float ceil)
{
	t_polygon	poly;

	poly = create_polygon(create_3dvertices_array(4), P_WALL, texture);
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

t_polygon	create_polygon(t_3dvertices *vertices, t_polygon_type type, SDL_Surface *texture)
{
	t_polygon	poly;

	ft_bzero(&poly, sizeof(t_polygon));
	poly.type = type;
	poly.vertices = vertices;
	poly.texture = texture;
	poly.lightmap = (uint8_t *)malloc(poly.texture->w * poly.texture->h * sizeof(uint8_t));
	ft_memset(poly.lightmap, 20, poly.texture->w * poly.texture->h * sizeof(uint8_t));
	return (poly);
}
