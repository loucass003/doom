/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_polygon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:31:56 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/05 02:04:53 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <stdlib.h>
#include "polygon.h"
#include "render.h"
#include "arrays.h"
#include "maths/line.h"

t_bool	add_polygon_vertice(t_renderable *r, t_vec4 v)
{
	if (!append_4dvertices_array(&r->vertices, v))
		return (FALSE);
	if (!append_2dvertices_array(&r->vertex, (t_vec2){ 0, 0 }))
		return (FALSE);
	return (TRUE);
}

t_bool	create_wall_polygon(t_renderable *r, t_mtl mtl, t_line line, t_vec2 bounds)
{
	if(!create_polygon(r, P_WALL, mtl))
		return (FALSE);
	add_polygon_vertice(r, (t_vec4){ line.a.x, bounds.x, line.a.y, 1 });
	add_polygon_vertice(r, (t_vec4){ line.b.x, bounds.x, line.b.y, 1 });
	add_polygon_vertice(r, (t_vec4){ line.b.x, bounds.y, line.b.y, 1 });
	add_polygon_vertice(r, (t_vec4){ line.a.x, bounds.y, line.a.y, 1 });
	if (!(r->pp_vertices = (t_vec4 *)malloc(sizeof(t_vec4) * r->vertices->len)))
		return (free_renderable(&r, FALSE));
	if (!(r->pp_normals = (t_vec3 *)malloc(sizeof(t_vec3) * r->normals->len)))
		return (free_renderable(&r, FALSE));
	return (TRUE);
}

t_bool	create_polygon(t_renderable *r, t_polygon_type type, t_mtl mtl)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_POLYGON;
	if (!(r->of.data.polygon = malloc(sizeof(t_polygon))))
		return (FALSE);
	r->of.data.polygon->type = type;
	if(!(r->vertices = create_4dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->vertex = create_2dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->normals = create_3dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->faces = create_faces_array(2)))
		return (free_renderable(&r, FALSE));
	if(!(r->materials = create_mtllist(3)))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, mtl))
		return (free_renderable(&r, FALSE));
	r->dirty = TRUE;
	r->fixed = TRUE;
	return (TRUE);
}
