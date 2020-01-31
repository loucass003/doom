/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable_door.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:22:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/31 20:25:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "render.h"
#include "editor.h"
#include "door.h"
#include "maths/mat4.h"

t_bool		create_door(t_doom *doom, int indexes[3], t_renderable *r)
{
	const t_editor			*editor = &doom->editor;
	const t_wall			wall = editor->rooms->values[indexes[0]].walls->values[indexes[1]];
	const t_wall_section	ws = wall.wall_sections->values[indexes[2]];
	
	t_vec3	v1 = vec4_to_3(get_map((t_editor *)editor)->vertices->vertices[ws.vertices_index[0]]);
	t_vec3	v2 = vec4_to_3(get_map((t_editor *)editor)->vertices->vertices[ws.vertices_index[1]]);
	t_vec3	vt1 = vec4_to_3(get_map((t_editor *)editor)->vertices->vertices[ws.vertices_index[2]]);
	t_vec3	vt2 = vec4_to_3(get_map((t_editor *)editor)->vertices->vertices[ws.vertices_index[3]]);

	copy_renderable(editor->doom->res_manager.ressources->values[22]->data.model, r);
	r->of.type = RENDERABLE_DOOR;
	if (!(r->of.data.door = ft_memalloc(sizeof(t_door))))
		return (FALSE);
	int		i;

	i = -1;
	while (++i < r->groups_count)
	{
		if (ft_strequ(r->groups[i], "door_part1"))
			r->of.data.door->door_1 = i;
		else if (ft_strequ(r->groups[i], "door_part2"))
			r->of.data.door->door_2 = i;
	}

	if (!(r->of.data.door->door_1_indexes = create_ints_array(100)))
		return (FALSE);
	if (!(r->of.data.door->door_2_indexes = create_ints_array(100)))
		return (FALSE);
	i = -1;
	while (++i < r->faces->len)
	{
		t_face face = r->faces->values[i];

		if (face.group == r->of.data.door->door_1)
		{
			int j = -1;

			while (++j < 3)
			{
				if (ints_indexof(r->of.data.door->door_1_indexes, face.vertices_index[j]) == -1)
					append_ints_array(&r->of.data.door->door_1_indexes, face.vertices_index[j]);
			}
		}
		else if (face.group == r->of.data.door->door_2)
		{
			int j = -1;

			while (++j < 3)
			{
				if (ints_indexof(r->of.data.door->door_2_indexes, face.vertices_index[j]) == -1)
					append_ints_array(&r->of.data.door->door_2_indexes, face.vertices_index[j]);
			}
		}
	}
	
	ft_memcpy(r->of.data.door->indexes, indexes, sizeof(int) * 4);
	r->scale = (t_vec3){ 1, 1, 30 };
	r->dirty = TRUE;
	t_vec3 right = ft_vec3_norm(ft_vec3_sub(v2, v1));
	float len = ft_vec3_len(ft_vec3_sub(v2, v1));
	r->scale.x = len;
	float start_y = fmax(v1.y, v2.y);
	float end_y = fmin(vt1.y, vt2.y);

	r->scale.y = end_y - start_y;
	r->position = ft_vec3_add(v1, ft_vec3_mul_s(right, len / 2));
	r->position.y = start_y;
	t_vec3 rot = rotation_matrix_to_euler(look_at(v2, ft_vec3_add(v2, ft_vec3_cross((t_vec3){ 0, 1, 0 }, right))));
	r->rotation = rot;
	
	return (TRUE);
}


void			update_renderable_door(t_render_context *ctx, t_renderable *r)
{
	t_door		*door = r->of.data.door;
	float		open_speed = 0.8 * ctx->doom->stats.delta;
	
	if (ctx->type != CTX_NORMAL)
		return ;
	float last_open = door->open_value;
	if (ft_vec3_len(ft_vec3_sub(r->position, ctx->doom->player.entity.position)) < 10)
	{
		if (door->open_value < 1)
			door->open_value += open_speed;
		if (door->open_value > 1)
			door->open_value = 1;
	}
	else
	{
		if (door->open_value > 0)
			door->open_value -= open_speed;
		if (door->open_value < 0)
			door->open_value = 0;
	}
	if (last_open != door->open_value)
	{
		int	i = -1;

		while (++i < r->faces->len)
		{
			t_face	face = r->faces->values[i];

			if (face.group == door->door_1 || face.group == door->door_2)
			{
				face.has_collision = door->open_value != 1;
			}
		}
		r->dirty = TRUE;
	}
}

void		transform_renderable_door(t_renderable *r)
{
	int		i;

	const	t_mat4 rot = ft_mat4_rotation(r->rotation);
	r->matrix = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(r->position),
			rot
		),
		ft_mat4_scale(r->scale)
	);


	t_mat4 m = ft_mat4_mul(r->matrix, ft_mat4_translation((t_vec3){ r->of.data.door->open_value, 0, 0 }));
	t_mat4 m2 = ft_mat4_mul(r->matrix, ft_mat4_translation((t_vec3){ -r->of.data.door->open_value, 0, 0 }));

	i = -1;
	while (++i < r->vertices->len)
	{
		t_mat4 a;
		
		if (ints_indexof(r->of.data.door->door_1_indexes, i + 1) != -1)
			a = m; 
		else if (ints_indexof(r->of.data.door->door_2_indexes, i + 1) != -1)
			a = m2;
		else
			a = r->matrix;
		//printf("LOL\n");
		r->pp_vertices[i] = mat4_mulv4(a, r->vertices->vertices[i]);
	}
	i = -1;
	while (++i < r->normals->len)
		r->pp_normals[i] = ft_mat4_mulv(rot, r->normals->vertices[i]);
	i = -1;
	while (++i < r->faces->len)
	{
		t_face *face = &r->faces->values[i];
		t_vec3	n = get_triangle_normal(
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]));
	
		if (face->normal_type == 0)
			face->face_normal = ft_vec3_inv(n);
		else
			face->face_normal = n;
	}
	compute_collidables(r);
}
