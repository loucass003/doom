/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_uvs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 00:50:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 20:19:14 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void		change_transform_mode(const SDL_Scancode key, t_doom *doom)
{
	if (key == SDL_SCANCODE_L)
	{
		doom->editor.uv_transform_mode++;
		if ((int)doom->editor.uv_transform_mode == 2)
			doom->editor.uv_transform_mode = 0;
	}
	else if (key == SDL_SCANCODE_P)
	{
		doom->editor.uv_transform_mode--;
		if ((int)doom->editor.uv_transform_mode == -1)
			doom->editor.uv_transform_mode = 1;
	}
}

void		uvs_mode(t_doom *doom, t_wall_section *ws, t_vec2 add, t_room *room)
{
	if (doom->editor.uv_transform_mode == UVT_MODE_OFFSET)
	{
		if (ws)
			ws->uv_offset = ft_vec2_add(ws->uv_offset, add);
		else
		{
			if (doom->editor.selected_floor_ceil == 0)
				room->floor_uv_offset = ft_vec2_add(room->floor_uv_offset, add);
			else
				room->ceil_uv_offset = ft_vec2_add(room->ceil_uv_offset, add);
		}
	}
	else if (doom->editor.uv_transform_mode == UVT_MODE_REPEAT)
	{
		if (ws)
			ws->uv_repeat = ft_vec2_add(ws->uv_repeat, add);
		else
		{
			if (doom->editor.selected_floor_ceil == 0)
				room->floor_uv_repeat = ft_vec2_add(room->floor_uv_repeat, add);
			else
				room->ceil_uv_repeat = ft_vec2_add(room->ceil_uv_repeat, add);
		}
	}
}

void		if_ws(t_doom *doom, int wi, t_wall_section *ws)
{
	if (ws)
		update_wall(&doom->editor, doom->editor.current_room, wi,
			doom->editor.wall_section);
	else
		add_map(get_map(&doom->editor), &doom->editor);
}

void		uvs_events(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;
	t_vec2				add;
	t_wall_section		*ws;
	t_room				*room;
	int					wi;

	add = (t_vec2){ 0, 0 };
	change_transform_mode(key, doom);
	if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN)
		add.y = (key == SDL_SCANCODE_UP ? 1 : -1) * 0.1;
	if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_RIGHT)
		add.x = (key == SDL_SCANCODE_RIGHT ? 1 : -1) * 0.1;
	if (add.x == 0 && add.y == 0)
		return ;
	room = &doom->editor.rooms->values[doom->editor.current_room];
	ws = NULL;
	if (doom->editor.current_seg.x != -1 && doom->editor.wall_section != -1
		&& (wi = wall_indexof_by_indice(room->walls,
			doom->editor.current_seg.x)) != -1)
		ws = &room->walls->values[wi].wall_sections
				->values[doom->editor.wall_section];
	uvs_mode(doom, ws, add, room);
	if_ws(doom, wi, ws);
}
