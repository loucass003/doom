/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 02:06:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"
#include "door.h"

t_vec2		uv_setting(t_wall_section *ws, t_vec2 uv)
{
	return (ft_vec2_add(ws->uv_offset, ft_vec2_mul(uv, ws->uv_repeat)));
}

t_bool		add_door(int j, int i, t_editor *editor, t_room *room)
{
	int				k;
	t_wall			*wall;
	t_wall_section	*ws;
	t_renderable	r;

	k = -1;
	wall = &room->walls->values[j];
	while (++k < wall->wall_sections->len)
	{
		ws = &wall->wall_sections->values[k];
		if (ws->type != WS_DOOR)
			continue;
		create_door(editor->doom, (int[3]){ i, j, k }, &r);
		post_process_renderable(editor->doom, &r, TRUE, FALSE);
		if (!append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		add_map(t_renderable *rmap, t_editor *editor)
{
	int		i;
	int		j;
	t_room	*room;

	//TODO: free removed renderables from map_renderable to renderables->len
	i = editor->map_renderable - 1;
	while (++i < editor->doom->renderables->len)
		free_renderable(&editor->doom->renderables->values[i], FALSE, TRUE, TRUE);
	editor->doom->renderables->len = editor->map_renderable;
	create_map(rmap, editor);
	if (!append_renderables_array(&editor->doom->renderables, *rmap))
		return (FALSE);
	hide_doors_wall(editor);
	i = -1;
	while (++i < editor->rooms->len)
	{
		j = -1;
		room = &editor->rooms->values[i];
		while (++j < room->walls->len)
			if (add_door(j, i, editor, room) == FALSE)
				return (FALSE);
	}
	return (TRUE);
}

t_bool		editor_setmap(t_editor *editor)
{
	int				i;
	t_renderable	r;
	t_object		*object;

	editor->doom->renderables->len = 0;
	editor->doom->skybox_index = -1;
	editor->settings.open = FALSE;
	default_renderables(editor->doom);
	i = -1;
	while (++i < editor->objects->len)
	{
		object = &editor->objects->values[i];
		if (object->type == OBJECT_NONE)
			continue ;
		if (!create_object_renderable(editor, i, &r)
			|| !append_renderables_array(&editor->doom->renderables, r))
			return (FALSE);
		object->r = &editor->doom->renderables
			->values[editor->doom->renderables->len - 1];
		object->r->has_hitbox ? object->r->show_hitbox = FALSE : 0;
	}
	editor->map_renderable = editor->doom->renderables->len;
	add_map(&r, editor);
	spawn_player(editor->doom);
	return (TRUE);
}
