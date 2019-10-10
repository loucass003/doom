/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_renderable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:17:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 01:24:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "polygon.h"
#include "sprite.h"
#include "render.h"

void		compute_sprite_hitbox(t_renderable *r)
{
	compute_ellipsoid_hitbox(r, r->position, r->of.data.sprite->hitbox_radius);
}

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

void	set_current_cell(t_renderable *r, int x, int y)
{
	t_sprite	*sprite;
	t_mtl		*mtl;
	t_vec2		div;
	t_vec2		pos;

	mtl = &r->materials->values[0];
	if (r->of.type != RENDERABLE_SPRITE || !mtl->texture_map_set)
		return ;
	sprite = r->of.data.sprite;
	pos = (t_vec2){ x, sprite->cells_count.y - 1 - y };
	div = (t_vec2){ mtl->texture_map->w / sprite->cells_count.x, mtl->texture_map->h / sprite->cells_count.y };
	sprite->is_spritesheet = TRUE;
	sprite->current_cell = y * sprite->cells_count.x + x;
	sprite->uv_min = ft_vec2_div(ft_vec2_mul(pos, div), (t_vec2){  mtl->texture_map->w, mtl->texture_map->h});
	sprite->uv_max = ft_vec2_div(ft_vec2_mul(ft_vec2_add(pos, (t_vec2){ 1, 1 }), div),  (t_vec2){  mtl->texture_map->w, mtl->texture_map->h});
	r->vertex->vertices[0] = sprite->uv_min;
	r->vertex->vertices[1] = (t_vec2){ sprite->uv_max.x, sprite->uv_min.y };
	r->vertex->vertices[2] = sprite->uv_max;
	r->vertex->vertices[3] = (t_vec2){ sprite->uv_min.x, sprite->uv_max.y };
}

t_bool	create_sprite(t_renderable *r, t_mtl mtl, t_vec2 cells_count)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_SPRITE;
	if (!(r->of.data.sprite = ft_memalloc(sizeof(t_sprite))))
		return (FALSE);
	r->of.data.sprite->always_facing_player = TRUE;
	r->of.data.sprite->cells_count = cells_count;
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
	add_point(r, (t_vec4){ -0.5, -0.5, 0, 1 }, (t_vec2){ 1, 0 });
	add_point(r, (t_vec4){ 0.5, -0.5, 0, 1 }, (t_vec2){ 1, 1 });
	add_point(r, (t_vec4){ 0.5, 0.5, 0, 1 }, (t_vec2){ 0, 1 });
	add_point(r, (t_vec4){ -0.5, 0.5, 0, 1 }, (t_vec2){ 0, 0 });
	if (!(r->pp_vertices = (t_vec4 *)malloc(sizeof(t_vec4) * r->vertices->len)))
		return (FALSE);
	if (!(r->pp_normals = (t_vec3 *)malloc(sizeof(t_vec3) * r->normals->len)))
		return (FALSE);
	triangulate_polygon(r);
	r->dirty = TRUE;
	r->fixed = FALSE;
	r->scale = (t_vec3){ 1, 1, 1 };
	r->double_faced = TRUE;
	return (TRUE);
}
