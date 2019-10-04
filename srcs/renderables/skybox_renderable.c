/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_renderable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 17:51:12 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/04 18:39:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "render.h"

t_bool	create_sprite(t_renderable *r, t_mtl mtl)
{
	ft_bzero(r, sizeof(t_renderable));
	// r->of.type = RENDERABLE_OBJ;
	// if(!(r->vertices = create_4dvertices_array(4)))
	// 	return (free_renderable(&r, FALSE));
	// if(!(r->vertex = create_2dvertices_array(4)))
	// 	return (free_renderable(&r, FALSE));
	// if(!(r->normals = create_3dvertices_array(8)))
	// 	return (free_renderable(&r, FALSE));
	// if(!(r->faces = create_faces_array(12)))
	// 	return (free_renderable(&r, FALSE));
	// if(!(r->materials = create_mtllist(1)))
	// 	return (free_renderable(&r, FALSE));
	// if (!append_mtllist(&r->materials, mtl))
	// 	return (free_renderable(&r, FALSE));
	// if (!(r->pp_vertices = (t_vec4 *)malloc(sizeof(t_vec4) * 4)))
	// 	return (FALSE);
	// if (!(r->pp_normals = (t_vec3 *)malloc(sizeof(t_vec3) * 4)))
	// 	return (FALSE);
	// add_point(r, (t_vec4){ -0.5, 0, 0, 1 }, (t_vec2){ 1, 0 });
	// add_point(r, (t_vec4){ 0.5, 0, 0, 1 }, (t_vec2){ 1, 1 });
	// add_point(r, (t_vec4){ 0.5, 1, 0, 1 }, (t_vec2){ 0, 1 });
	// add_point(r, (t_vec4){ -0.5, 1, 0, 1 }, (t_vec2){ 0, 0 });
	// triangulate_polygon(r);
	// r->dirty = TRUE;
	// r->fixed = FALSE;
	// r->scale = (t_vec3){ 1, 1, 1 };
	// r->double_faced = TRUE;
	return (TRUE);
}