/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_ceil_floor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:58:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/14 18:32:40 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "gui.h"
#include "editor.h"

void		ceil_floor_slope(t_doom *doom, t_room *room, int i, t_mat4 m_rot)
{
	t_wall	*wall;
	t_vec2	v;
	t_vec3	point;

	wall = &room->walls->values[i];
	v = doom->editor.points->vertices[wall->indice];
	point = ft_mat4_mulv(m_rot, editor_to_world((t_vec3){ v.x, 0, v.y }));
	if (doom->editor.selected_floor_ceil == 0)
		wall->floor_height = point.y;
	else
		wall->ceiling_height = point.y;
}

void		ceil_floor_select(t_doom *doom)
{
	add_map(&doom->renderables->values[doom->editor.map_renderable],
		&doom->editor);
	select_floor_ceil(&doom->editor, doom->editor.current_room,
		doom->editor.selected_floor_ceil == 0);
}

void		ceil_floor_rot(t_doom *doom, t_room *room, SDL_Scancode key)
{
	t_vec3	rot;
	t_vec2	first_point;
	t_mat4	m_rot;
	int		i;

	rot = doom->editor.selected_floor_ceil == 0 ? room->floor_rot
		: room->ceil_rot;
	if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6)
		rot.x += 0.01 * (key == SDL_SCANCODE_KP_4 ? 1 : -1);
	if (key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
		rot.z += 0.01 * (key == SDL_SCANCODE_KP_8 ? 1 : -1);
	rot.x = clamp(-M_PI / 4, M_PI / 4, rot.x);
	rot.z = clamp(-M_PI / 4, M_PI / 4, rot.z);
	first_point = doom->editor.points->vertices[room->walls->values[0].indice];
	m_rot = ft_mat4_mul(ft_mat4_translation(ft_vec3_inv((t_vec3){ first_point.x,
		0, first_point.y })), ft_mat4_rotation(rot));
	i = -1;
	while (++i < room->walls->len)
		ceil_floor_slope(doom, room, i, m_rot);
	if (doom->editor.selected_floor_ceil == 0)
		room->floor_rot = rot;
	else
		room->ceil_rot = rot;
	ceil_floor_select(doom);
}

void		ceil_floor_height(t_doom *doom, t_room *room, SDL_Scancode key,
	int i)
{
	t_wall	*wall;
	float	add;

	wall = &room->walls->values[i];
	add = 0.1 * (key == SDL_SCANCODE_KP_PLUS ? -1 : 1);
	if (doom->editor.selected_floor_ceil == 0)
		wall->floor_height += add;
	else
		wall->ceiling_height += add;
}

void		ceil_floor_events(t_doom *doom, SDL_Event *event)
{
	t_room				*room;
	const SDL_Scancode	key = event->key.keysym.scancode;
	int					i;

	if (!doom->editor.slope_mode)
		return ;
	i = -1;
	room = &doom->editor.rooms->values[doom->editor.current_room];
	if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6
		|| key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
		ceil_floor_rot(doom, room, key);
	else if (key == SDL_SCANCODE_KP_PLUS || key == SDL_SCANCODE_KP_MINUS)
	{
		while (++i < room->walls->len)
			ceil_floor_height(doom, room, key, i);
		add_map(&doom->renderables->values[doom->editor.map_renderable],
			&doom->editor);
		select_floor_ceil(&doom->editor, doom->editor.current_room,
			doom->editor.selected_floor_ceil == 0);
	}
}
