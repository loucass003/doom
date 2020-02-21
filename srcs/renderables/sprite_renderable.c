/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_renderable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:17:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/21 16:49:15 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "render.h"
#include "obj.h"

void			compute_sprite_hitbox(t_renderable *r)
{
	compute_ellipsoid_hitbox(r, r->position, r->sprite->hitbox_radius);
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

void			set_current_cell(t_renderable *r, int x, int y)
{
	t_sprite	*sprite;
	t_mtl		*mtl;
	t_vec2		div;
	t_vec2		pos;

	mtl = &r->materials->values[0];
	if (!r->sprite || !mtl->texture_map_set)
		return ;
	sprite = r->sprite;
	pos = (t_vec2){ x, sprite->cells_count.y - 1 - y };
	div = (t_vec2){ mtl->texture_map->width / sprite->cells_count.x,
		mtl->texture_map->height / sprite->cells_count.y };
	sprite->current_cell = y * sprite->cells_count.x + x;
	sprite->uv_min = ft_vec2_div(ft_vec2_mul(pos, div), (t_vec2){
		mtl->texture_map->width, mtl->texture_map->height});
	sprite->uv_max = ft_vec2_div(ft_vec2_mul(ft_vec2_add(pos, (t_vec2){
		1, 1 }), div), (t_vec2){
		mtl->texture_map->width, mtl->texture_map->height});
	r->vertex->vertices[0] = sprite->uv_min;
	r->vertex->vertices[1] = (t_vec2){ sprite->uv_max.x, sprite->uv_min.y };
	r->vertex->vertices[2] = sprite->uv_max;
	r->vertex->vertices[3] = (t_vec2){ sprite->uv_min.x, sprite->uv_max.y };
}

t_sprite		*create_sprite(t_vec2 cells_count, t_ressource *texture)
{
	t_sprite	*sprite;

	if (!(sprite = ft_memalloc(sizeof(t_sprite))))
		return (NULL);
	sprite->always_facing_player = TRUE;
	sprite->cells_count = cells_count;
	sprite->texture = texture;
	return (sprite);
}

t_bool			create_sprite_renderable(t_renderable *r, t_sprite *sprite)
{
	if (!create_renderable(r, RENDERABLE_UNKNOWN))
		return (FALSE);
	r->object_index = -1;
	r->sprite = sprite;
	if (!append_mtllist(&r->materials, (t_mtl){
		.texture_map = sprite->texture->data.texture,
		.texture_map_set = TRUE }))
		return (free_renderable(r, FALSE, FALSE));
	add_point(r, (t_vec4){ -0.5, -0.5, 0, 1 }, (t_vec2){ 1, 0 });
	add_point(r, (t_vec4){ 0.5, -0.5, 0, 1 }, (t_vec2){ 0, 0 });
	add_point(r, (t_vec4){ 0.5, 0.5, 0, 1 }, (t_vec2){ 0, 1 });
	add_point(r, (t_vec4){ -0.5, 0.5, 0, 1 }, (t_vec2){ 1, 1 });
	if (!append_faces_array(&r->faces, create_face((int[3][3]){ {1, 4, 3},
		{1, 4, 3}, {1, 4, 3} }, 0, 1)))
		return (FALSE);
	if (!append_faces_array(&r->faces, create_face((int[3][3]){ {1, 3, 2},
		{1, 3, 2}, {1, 3, 2} }, 0, 1)))
		return (FALSE);
	r->double_faced = TRUE;
	r->dirty = TRUE;
	r->fixed = FALSE;
	r->scale = (t_vec3){ 1, 1, 1 };
	post_process_renderable(NULL, r, FALSE, FALSE);
	return (TRUE);
}
