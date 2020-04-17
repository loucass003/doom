/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 19:39:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "sprite.h"
#include "render.h"
#include "octree.h"
#include "door.h"
#include "explosion.h"

void	render(t_doom *doom)
{
	t_gui	*gui;

	if (doom->current_gui < 0)
		return ;
	gui = &doom->guis[doom->current_gui];
	gui->render(gui, doom);
}

static t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	return (r);
}

float	get_lights_intensity(t_render_context *ctx, t_renderable *r,
	t_vec3 normal, t_vec4 point, t_face *face)
{
	uint8_t	ambiant;
	int		i;
	float	sum;

	if (!ctx->doom->lights || (r && (r->no_light)))
		return (255);
	ambiant = AMBIANT_LIGHT;
	if (r->of.type == RENDERABLE_MAP && face->room_index != -1)
		ambiant = ctx->doom->editor.rooms->values[face->room_index].ambiant_light;
	i = -1;
	sum = 0;
	while (++i < ctx->doom->lights->len)
	{
		t_vec3	dir;
		float	intensity;
		t_light	*light;
		light = &ctx->doom->lights->values[i];
		dir = ft_vec3_norm(ft_vec3_sub(vec4_to_3(point), light->position));
		intensity = ft_vec3_dot(dir, ft_vec3_inv(normal));
		if (light->type == LIGHT_SPOT)
		 	intensity = 1.0 / ((1.0 - ((ft_vec3_dot(dir, light->dir) + 1.0) / 2.0)) * 10.0) * 0.2;
		sum += (intensity) * (15 / ft_vec3_len(ft_vec3_sub(vec4_to_3(point), light->position))) * light->intensity;
	}
	return clamp(ambiant, 255, sum);
}

typedef struct	s_face_data
{
	t_render_context	*ctx;
	t_renderable		*r;
}				t_face_data;

t_bool	prepare_face(t_face *face, t_mtl *mtl, t_renderable *r,
	t_render_context *ctx)
{
	if (r->no_collision)
		face->has_collision = FALSE;
	if (r->double_faced)
		face->double_sided = TRUE;
	if (r->wireframe)
		mtl->wireframe = TRUE;
	if (r->wireframe_color == 0)
		r->wireframe_color = 0xFF555555;
	if (mtl->wireframe || (!mtl->texture_map_set && !mtl->material_color_set))
	{
		mtl->material_color_set = TRUE;
		mtl->material_color = mtl->wireframe ? r->wireframe_color : 0xFF555555;
	}
	if (!face->double_sided
		&& ft_vec3_dot(face->face_normal, ft_vec3_sub(ctx->camera->pos,
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]))) <= 0)
		return (FALSE);
	face->rendered = TRUE;
	return (TRUE);
}

void	render_face(int face_index, void *p)
{
	t_face_data			*face_data;

	t_renderable		*r;
	t_render_context	*ctx;
	t_face				*face;
	t_mtl				*mtl;

	face_data = p;

	r = face_data->r;
	ctx = face_data->ctx;
	face = &r->faces->values[face_index];
	if (face->hidden || face->rendered)
		return ;
	mtl = &r->materials->values[face->mtl_index];
	if (!prepare_face(face, mtl, r, ctx))
		return ;

	t_vec4	v[3];
	float	it[3];
	t_vec2	vertex[3];

	it[0] = get_lights_intensity(ctx, r, r->pp_normals[face->normals_index[0] - 1], r->pp_vertices[face->vertices_index[0] - 1], face);
	it[1] = get_lights_intensity(ctx, r, r->pp_normals[face->normals_index[1] - 1], r->pp_vertices[face->vertices_index[1] - 1], face);
	it[2] = get_lights_intensity(ctx, r, r->pp_normals[face->normals_index[2] - 1], r->pp_vertices[face->vertices_index[2] - 1], face);

	v[0] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[0] - 1]));
	v[1] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[1] - 1]));
	v[2] = mat4_mulv4(ctx->camera->projection,
		mat43_mulv4(ctx->camera->matrix,
		r->pp_vertices[face->vertices_index[2] - 1]));

	vertex[0] = (t_vec2){0,0};
	vertex[1] = (t_vec2){0,0};
	vertex[2] = (t_vec2){0,0};
	if (r->vertex)
	{
		vertex[0] = r->vertex->vertices[face->vertex_index[0] - 1];
		vertex[1] = r->vertex->vertices[face->vertex_index[1] - 1];
		vertex[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	}
	process_triangle(ctx, mtl, (t_triangle) {
		{ .pos = v[0], .tex = vertex[0], .normal = r->pp_normals[face->normals_index[0] - 1], .light_color = it[0] },
		{ .pos = v[1], .tex = vertex[1], .normal = r->pp_normals[face->normals_index[1] - 1], .light_color = it[1] },
		{ .pos = v[2], .tex = vertex[2], .normal = r->pp_normals[face->normals_index[2] - 1], .light_color = it[2] }
	});
}

void	update_renderable_entity_enemy(t_render_context *ctx, t_renderable *r,
	t_entity *entity)
{
	t_sprite		*sprite;
	t_entity_enemy	*enemy;
	t_vec3			view;
	float			a;
	int				texture;

	enemy = &entity->of.enemy;
	sprite = r->sprite;
	view = ft_vec3_sub(ctx->camera->pos, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	a = atan2(view.z, view.x);
	texture = round(a * (4 / M_PI));
	if (enemy->animation_step == 6)
		texture = enemy->shooting ? 2 : 1;
	if (entity->diying)
		texture = enemy->diying_step;
	set_current_cell(r, texture, enemy->animation_step);
}

void	update_renderable_entity_boss(t_render_context *ctx, t_renderable *r,
	t_entity *entity)
{
	t_entity_boss	*boss;

	boss = &entity->of.boss;
	set_current_cell(r, boss->animation_step, boss->phase);
}

t_bool	update_renderable_entity(t_render_context *ctx, t_renderable *r)
{
	t_entity	*entity;

	entity = r->of.data.entity;
	r->position = entity->position;
	r->rotation = entity->rotation;
	r->dirty = TRUE;
	if (entity->type == ENTITY_ENEMY)
		update_renderable_entity_enemy(ctx, r, entity);
	else if (entity->type == ENTITY_BOSS)
		update_renderable_entity_boss(ctx, r, entity);
	return (TRUE);
}

t_bool	update_renderable(t_render_context *ctx, t_renderable *r)
{
	if (r->of.type == RENDERABLE_ENTITY)
		return (update_renderable_entity(ctx, r));
	else if (r->of.type == RENDERABLE_DOOR)
		return (update_renderable_door(ctx, r));
	else if (r->of.type == RENDERABLE_EXPLOSION)
		return (update_renderable_explosion(ctx, r));
	return (TRUE);
}

t_bool	pre_render(t_render_context *ctx, t_renderable *r)
{
	int	i;

	if (!update_renderable(ctx, r))
		return (FALSE);
	if (r->dirty || (r->sprite && r->sprite->always_facing_player))
	{
		if (r->sprite && r->sprite->always_facing_player)
			r->rotation.y = ctx->camera->rotation.y;
		transform_renderable(r);
		r->dirty = FALSE;
	}
	if (!r->visible)
		return (TRUE);
	i = -1;
	while (++i < r->faces->len)
		r->faces->values[i].rendered = FALSE;
	return (TRUE);
}

t_bool	render_renderable(t_render_context *ctx, t_renderable *r)
{
	int			i;
	t_face_data	face_data;

	if (!pre_render(ctx, r))
		return (FALSE);
	face_data = (t_face_data){ .ctx = ctx, .r = r };
	if (r->octree)
	{
		frustum_to_local(ctx->camera, r);
		frustum_intersect_octree(r->octree, ctx->camera->frustum,
			render_face, &face_data);
	}
	else
	{
		i = -1;
		while (++i < r->faces->len)
			render_face(i, &face_data);
	}
	return (TRUE);
}
