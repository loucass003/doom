/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable_door.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:22:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/30 15:42:58 by llelievr         ###   ########.fr       */
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
	if (!(r->of.data.door = malloc(sizeof(t_door))))
		return (FALSE);
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
