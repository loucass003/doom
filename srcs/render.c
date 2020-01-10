/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/10 03:58:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "sprite.h"
#include "render.h"
#include "octree.h"

void	render(t_doom *doom)
{
/*	ft_bzero(doom->rendered_area, doom->screen.width);
	traverseDrawOrder(doom, doom->bsp);*/
	if (doom->current_gui < 0)
		return ;
	const t_gui		*gui = &doom->guis[doom->current_gui];
	gui->render((t_gui *)gui, doom);
}

static t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	//r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}

float	get_light_intensity(t_render_context *ctx, t_renderable *r, t_vec3 normal, t_vec4 point)
{
	if (!ctx->doom->lights || (r && r->no_light))
		return (255);

	int		i;
	float	sum;
	int		valid;
	t_light	*light;
	
	i = -1;
	sum = 0;
	valid = 0;
	while (++i < ctx->doom->lights->len)
	{
		light = &ctx->doom->lights->values[i];
		//float d = ft_vec3_dot(normal, ft_vec3_sub(light->position, vec4_to_3(point)));
		// if (d > 5)
		// 	continue;
		t_vec3 dir = ft_vec3_norm(ft_vec3_sub(vec4_to_3(point), light->position));
		if (ft_vec3_dot(dir, light->dir) >= cosf(M_PI / 10))
		{
			valid++;
			sum += ft_max(AMBIANT_LIGHT, clamp(0, 1, ft_vec3_dot(dir, ft_vec3_inv(normal)) * (15 / ft_vec3_len(ft_vec3_sub(vec4_to_3(point), light->position)))) * 255);
		}
	}
	if (valid == 0)
		return AMBIANT_LIGHT;
	return sum / (float)valid;
}

typedef struct	s_face_data
{
	t_render_context	*ctx;
	t_renderable		*r;
}				t_face_data;


int faces_count = 0;

void	render_face(int face_index, void *p)
{
	//printf("CALL\n");
	t_face_data *face_data = p;
	t_vec2	vertex = (t_vec2){ 0, 0 };
	t_renderable *r = face_data->r;
	t_render_context *ctx = face_data->ctx;
	t_face *face = &r->faces->values[face_index];
	t_mtl *mtl = &r->materials->values[face->mtl_index];
	if (face->hidden || face->rendered)
		return;
	
	faces_count++;
	if (r->no_collision)
		face->has_collision = FALSE;
	if (r->double_faced)
		face->double_sided = TRUE;
	if (r->wireframe)
		mtl->wireframe = TRUE;

	if (r->wireframe_color == 0)
		r->wireframe_color =  0xFF555555;

	if (mtl->wireframe || (!mtl->texture_map_set && !mtl->material_color_set))
	{
		mtl->material_color_set = TRUE;
		mtl->material_color = mtl->wireframe ? r->wireframe_color : 0xFF555555;
	}

	if (!face->double_sided)
	{
		float d = ft_vec3_dot(face->face_normal, ft_vec3_sub(ctx->camera->pos, vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1])));
		if (d <= 0)
			return;
	}

	face->rendered = TRUE;

	t_vec4 v0 = mat43_mulv4(ctx->camera->matrix, r->pp_vertices[face->vertices_index[0] - 1]);
	t_vec4 v1 = mat43_mulv4(ctx->camera->matrix, r->pp_vertices[face->vertices_index[1] - 1]);
	t_vec4 v2 = mat43_mulv4(ctx->camera->matrix, r->pp_vertices[face->vertices_index[2] - 1]);

	v0 = mat4_mulv4(ctx->camera->projection, v0);
	v1 = mat4_mulv4(ctx->camera->projection, v1);
	v2 = mat4_mulv4(ctx->camera->projection, v2);

	float it0 = get_light_intensity(ctx, r, r->pp_normals[face->normals_index[0] - 1], r->pp_vertices[face->vertices_index[0] - 1]);
	float it1 = get_light_intensity(ctx, r, r->pp_normals[face->normals_index[1] - 1], r->pp_vertices[face->vertices_index[1] - 1]);
	float it2 = get_light_intensity(ctx, r, r->pp_normals[face->normals_index[2] - 1], r->pp_vertices[face->vertices_index[2] - 1]);
	
	t_vec2 vertex0 = vertex;
	t_vec2 vertex1 = vertex;
	t_vec2 vertex2 = vertex;

	if (r->vertex)
	{
		vertex0 = r->vertex->vertices[face->vertex_index[0] - 1];
		vertex1 = r->vertex->vertices[face->vertex_index[1] - 1];
		vertex2 = r->vertex->vertices[face->vertex_index[2] - 1];
	}

	process_triangle(ctx, mtl, (t_triangle) {
		{ .pos = v0, .tex = vertex0, .normal = r->pp_normals[face->normals_index[0] - 1], .light_color = it0 },
		{ .pos = v1, .tex = vertex1, .normal = r->pp_normals[face->normals_index[1] - 1], .light_color = it1 },
		{ .pos = v2, .tex = vertex2, .normal = r->pp_normals[face->normals_index[2] - 1], .light_color = it2 }
	});
}


void	render_renderable(t_render_context *ctx, t_renderable *r)
{
	int		i;
	// t_face	*face;
	// t_mtl	*mtl;


	if (r->of.type == RENDERABLE_ENTITY)
	{
		t_entity *entity = r->of.data.entity;
		r->position = entity->position;
		r->rotation = entity->rotation;
		r->dirty = TRUE;
		if(entity->type == ENTITY_ENEMY)
		{
			t_sprite	*sprite;
			t_entity_enemy	*enemy;

			enemy = &entity->of.enemy;
			sprite = r->sprite;
			t_vec3 view = ft_vec3_sub(ctx->camera->pos, entity->position);
			view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
			float a = atan2(view.z, view.x);
			float f = round(a * (4 / M_PI));
			int texture = f;
			if (enemy->animation_step == 6)
				texture = enemy->shooting ? 2 : 1;
			if (enemy->diying)
				texture = enemy->diying_step;
			set_current_cell(r, texture, enemy->animation_step);
		}
		else if (entity->type == ENTITY_BOSS)
		{
			t_entity_boss	*boss;

			boss = &entity->of.boss;
			set_current_cell(r, boss->animation_step, boss->phase);
		}
		
	}

	if (r->dirty || (r->sprite && r->sprite->always_facing_player))
	{
		if (r->sprite && r->sprite->always_facing_player)
			r->rotation.y = ctx->camera->rotation.y;
		transform_renderable(r);
		r->dirty = FALSE;
	}
	
	
	if (!r->visible)
		return ;
	t_face_data face_data = (t_face_data){ .ctx = ctx, .r = r };

	i = -1;
	while (++i < r->faces->len)
		r->faces->values[i].rendered = FALSE;
	faces_count = 0;
	

	if (r->octree)
	{
		frustum_to_local(ctx->camera, r);
		frustum_intersect_octree(r->octree, ctx->camera->frustum, render_face, &face_data);
		// printf("\nOCTREE %d %d/%d\n\n", r->of.type, faces_count, r->faces->len);
	}
	else
	{
		i = -1;
		while (++i < r->faces->len)
			render_face(i, &face_data);
//		printf("NORMAL %d %d/%d\n", r->of.type, faces_count, r->faces->len);
	}
	//r->wireframe = TRUE;

	
	//r->wireframe = FALSE;
}
