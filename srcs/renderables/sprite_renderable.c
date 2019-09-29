/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_renderable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:17:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/28 21:56:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "render.h"

static t_bool	add_point(t_renderable *r, t_vec4 v, t_vec2 vertex)
{
	if (!append_4dvertices_array(&r->vertices, v))
		return (FALSE);
	if (!append_2dvertices_array(&r->vertex, vertex))
		return (FALSE);
	if (!append_3dvertices_array(&r->normals, (t_vec3){ 0, 0, -1 }))
		return (FALSE);
	return (TRUE);
}

t_bool	create_sprite(t_renderable *r, t_mtl mtl)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_SPRITE;
	if (!(r->of.data.sprite = ft_memalloc(sizeof(t_sprite))))
		return (FALSE);
	r->of.data.sprite->always_facing_player = TRUE;
	if(!(r->vertices = create_4dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->vertex = create_2dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->normals = create_3dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->faces = create_faces_array(2)))
		return (free_renderable(&r, FALSE));
	if(!(r->materials = create_mtllist(1)))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, mtl))
		return (free_renderable(&r, FALSE));
	if (!(r->pp_vertices = (t_vec4 *)malloc(sizeof(t_vec4) * 4)))
		return (FALSE);
	if (!(r->pp_normals = (t_vec3 *)malloc(sizeof(t_vec3) * 4)))
		return (FALSE);
	add_point(r, (t_vec4){ -0.5, 0, 0, 1 }, (t_vec2){ 1, 0 });
	add_point(r, (t_vec4){ 0.5, 0, 0, 1 }, (t_vec2){ 1, 1 });
	add_point(r, (t_vec4){ 0.5, 1, 0, 1 }, (t_vec2){ 0, 1 });
	add_point(r, (t_vec4){ -0.5, 1, 0, 1 }, (t_vec2){ 0, 0 });
	triangulate_polygon(r);
	r->dirty = TRUE;
	r->fixed = FALSE;
	r->scale = (t_vec3){ 1, 1, 1 };
	return (TRUE);
}
