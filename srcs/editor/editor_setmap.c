/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/15 16:20:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"

t_bool		create_room_mesh(t_renderable *r, t_editor *editor, t_room *room)
{
	int		i;
	t_wall	*wall;
	t_vec2	point;

	i = -1;
	while (++i < room->walls->len)
	{
		wall = &room->walls->values[i];
		point = editor->points->vertices[wall->indice];
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, 0, point.y, 1 });
		append_4dvertices_array(&r->vertices, (t_vec4){ point.x, 1, point.y, 1 });
	}

}

t_bool		create_room_renderable(t_renderable *r, t_editor *editor, t_room *room)
{
	if (!create_renderable(r, RENDERABLE_UNKNOWN))
		return (FALSE);
	if(!(r->materials = create_mtllist(1)))
		return (free_renderable(&r, FALSE));
	if (!append_mtllist(&r->materials, (t_mtl){ 
			.material_color_set = TRUE, .material_color = 0xFFFF00FF }))
		return (free_renderable(&r, FALSE));
	create_room_mesh(r, editor, room);
	return (TRUE);
}

t_bool		editor_setmap(t_editor *editor)
{
	int		i;

	editor->doom->renderables->len = 0;
	while (++i < editor->rooms->len)
	{
		t_room			*room = &editor->rooms->values[i];
		t_renderable	r;

		if (!room->closed)
			continue;
		if (!create_room_renderable(&r, editor, room))
			return (FALSE);
		if (!append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
	}
	return (TRUE);
}
