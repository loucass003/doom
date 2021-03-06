/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/21 00:24:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
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

	i = editor->map_renderable - 1;
	while (++i < editor->doom->renderables->len)
		free_renderable(&editor->doom->renderables->values[i],
			FALSE, TRUE);
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

void		update_editor_var(t_editor *editor)
{
	editor->doom->renderables->len = 0;
	editor->doom->skybox_index = -1;
	editor->settings.open = TRUE;
}

t_bool		editor_setmap(t_editor *editor)
{
	t_renderable	r;

	update_editor_var(editor);
	default_renderables(editor->doom);
	add_objects(editor);
	editor->map_renderable = editor->doom->renderables->len;
	update_rooms_gaps(editor);
	add_map(&r, editor);
	spawn_player(editor->doom);
	return (TRUE);
}
