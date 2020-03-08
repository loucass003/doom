/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:46:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/03 20:06:55 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

void			remove_point(t_editor *editor, int index)
{
	int		i;
	t_walls	*walls;
	int		*v;
	int		j;

	splice_2dvertices_array(editor->points, index, 1);
	i = -1;
	while (++i < editor->rooms->len)
	{
		j = -1;
		walls = editor->rooms->values[i].walls;
		while (++j < walls->len)
		{
			v = &walls->values[j].indice;
			if (*v > index)
				*v = *v - 1;
		}
	}
}

void			insert_point(t_editor *editor, t_vec2 seg, int point_index)
{
	int		i;
	int		tmp;
	int		index1;
	int		index0;
	t_room	*room0;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room0 = &editor->rooms->values[i];
		if (!room0->closed)
			continue;
		index0 = wall_indexof_by_indice(room0->walls, seg.x);
		index1 = wall_indexof_by_indice(room0->walls, seg.y);
		if (index0 > index1)
		{
			tmp = index0;
			index0 = index1;
			index1 = tmp;
		}
		if (index0 != -1 && index1 != -1)
			point_room(index1, room0, point_index, index0);
	}
	update_rooms_gaps(editor);
}

t_bool			is_in_range(t_vec2 pos, t_vec2 test)
{
	return ((test.x - pos.x) * (test.x - pos.x) + (test.y - pos.y)
		* (test.y - pos.y) < 9 * 9);
}

t_bool			search_room_point(t_editor *editor, t_vec2 pos,
	t_vec2 *p)
{
	int		j;
	t_room	*room2;
	t_bool	found;

	j = -1;
	while (++j < editor->rooms->len)
	{
		room2 = &editor->rooms->values[j];
		found = FALSE;
		*p = point_on_room(editor, room2, pos, &found);
		if (found)
			return (TRUE);
	}
	return (FALSE);
}

t_vec2			get_close_point(t_editor *editor, t_vec2 pos)
{
	t_vec2	p;
	

	p = pos;
	if ((editor->selected_tool == TOOL_OBJECTS || editor->selected_tool
		== TOOL_SELECT) && get_close_object(editor, &p))
		return (p);
	if (search_room_point(editor, pos, &p) == TRUE)
		return (p);
	if (editor->player_set
		&& is_in_range((t_vec2){editor->doom->player.spawn_data.position.x,
		editor->doom->player.spawn_data.position.z}, p))
	{
		editor->grid_cell_grab = GG_PLAYER;
		return ((t_vec2){editor->doom->player.spawn_data.position.x,
		editor->doom->player.spawn_data.position.z});
	}
	editor->grid_cell_grab = GG_NONE;
	if (!in_bounds((SDL_Rect){ 10, 70, S_WIDTH - 20, S_HEIGHT - 80 }, pos)
		|| (is_settings_open(editor) && in_bounds((SDL_Rect){ S_WIDTH - 335,
		75, 320, 550 }, pos)))
		editor->grid_cell_grab = GG_OUTSIDE;
	return (pos);
}
