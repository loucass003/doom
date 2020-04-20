/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_renderable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:22:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 23:18:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "render.h"
#include "editor.h"
#include "door.h"
#include "maths/mat4.h"

void		filter_door_side(t_face face, t_ints **door_indexes)
{
	int j;

	j = -1;
	while (++j < 3)
	{
		if (ints_indexof(*door_indexes, face.vertices_index[j]) == -1)
			append_ints_array(&*door_indexes, face.vertices_index[j]);
	}
}

t_bool		create_door_faces_index(t_renderable *r, int indexes[3])
{
	int			i;
	t_face		face;

	i = -1;
	while (++i < (int)r->groups_count)
	{
		if (ft_strequ(r->groups[i], "door_part1"))
			r->of.data.door->door_1 = i;
		else if (ft_strequ(r->groups[i], "door_part2"))
			r->of.data.door->door_2 = i;
	}
	if (!(r->of.data.door->door_1_indexes = create_ints_array(100))
		|| !(r->of.data.door->door_2_indexes = create_ints_array(100)))
		return (FALSE);
	i = -1;
	while (++i < r->faces->len)
	{
		face = r->faces->values[i];
		if (face.group == r->of.data.door->door_1)
			filter_door_side(face, &r->of.data.door->door_1_indexes);
		else if (face.group == r->of.data.door->door_2)
			filter_door_side(face, &r->of.data.door->door_2_indexes);
	}
	ft_memcpy(r->of.data.door->indexes, indexes, sizeof(int) * 3);
	return (TRUE);
}

void		compute_door_postiton(t_editor *editor, t_renderable *r,
	t_wall_section ws)
{
	t_vec3			v[2];
	t_vec3			vt[2];
	t_vec2			bounds;
	t_vec3			right;
	t_renderable	*map;

	map = get_map((t_editor *)editor);
	v[0] = vec4_to_3(map->vertices->vertices[ws.vertices_index[0]]);
	v[1] = vec4_to_3(map->vertices->vertices[ws.vertices_index[1]]);
	vt[0] = vec4_to_3(map->vertices->vertices[ws.vertices_index[2]]);
	vt[1] = vec4_to_3(map->vertices->vertices[ws.vertices_index[3]]);
	right = ft_vec3_norm(ft_vec3_sub(v[1], v[0]));
	r->scale.x = ft_vec3_len(ft_vec3_sub(v[1], v[0]));
	bounds.x = fmax(v[0].y, v[1].y) - 0.5;
	bounds.y = fmin(vt[0].y, vt[1].y);
	r->scale.y = bounds.y - bounds.x;
	r->position = ft_vec3_add(v[0], ft_vec3_mul_s(right, r->scale.x / 2));
	r->position.y = bounds.x;
	r->rotation = rotation_matrix_to_euler(look_at(v[1], ft_vec3_add(v[1],
		ft_vec3_cross((t_vec3){ 0, 1, 0 }, right))));
}

t_bool		create_door(t_doom *doom, int indexes[3], t_renderable *r)
{
	const t_editor			*editor = &doom->editor;
	t_wall_section			ws;

	ws = editor->rooms->values[indexes[0]].walls
		->values[indexes[1]].wall_sections->values[indexes[2]];
	copy_renderable(editor->doom->res_manager.ressources
		->values[22]->data.model, r);
	r->of.type = RENDERABLE_DOOR;
	if (!(r->of.data.door = ft_memalloc(sizeof(t_door))))
		return (FALSE);
	if (!create_door_faces_index(r, indexes))
		return (FALSE);
	r->scale = (t_vec3){ 1, 1, 30 };
	r->dirty = TRUE;
	compute_door_postiton((t_editor *)editor, r, ws);
	return (TRUE);
}

void		free_door(t_door **d_addr)
{
	t_door	*door;

	if (!(*d_addr))
		return ;
	door = *d_addr;
	ft_memdel((void **)&door->door_1_indexes);
	ft_memdel((void **)&door->door_2_indexes);
	ft_memdel((void **)d_addr);
}
