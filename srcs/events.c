/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/11 13:32:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_node	*get_player_node(t_doom *doom)
{
	const t_vec2	p = (t_vec2){doom->player.pos.x, doom->player.pos.z};
	t_node			*n;

	n = doom->bsp;
	while (n->type != N_LEAF)
	{
		if (get_side_thin(n->partition, p) == S_FRONT)
			n = n->front;
		else
			n = n->back;
	}
	return (n);
}

void	up_down_polygon(t_polygon *poly, t_bool up)
{
	if (poly->type == P_CEILING)
	{
		int j = -1;
		while (++j < poly->vertices->len)
			poly->vertices->vertices[j].y += 0.05 * (up ? 1 : -1);
	}

	if (poly->type == P_WALL)
	{
		int l = -1;
		int len = floor(poly->indices->len / 3.);
		while (++l < len)
		{
			if (l % 2 == 0)
				poly->vertices->vertices[poly->indices->values[l * 3 + 1]].y += 0.05 * (up ? 1 : -1);
			else
			{
				poly->vertices->vertices[poly->indices->values[l * 3]].y += 0.05 * (up ? 1 : -1);
			}
		}
	}
}

void	rotate_polygon(t_polygon *poly, t_player *player, int axis)
{
	printf("axis %d\n", ft_abs(axis) - 1);
	const t_vec2		rot = player->rotation;
	const t_vec2		dir[2] = { { cosf(rot.y), sinf(rot.y) }, 
		{ cosf(rot.y + M_PI_2), sinf(rot.y + M_PI_2) }};
	t_line				part = (t_line){ .a = {player->pos.x, player->pos.z}, 
		.b = {player->pos.x + dir[ft_abs(axis) - 1].x,
		player->pos.z + dir[ft_abs(axis) - 1].y}
	};
	t_side				side;

	part.normal = line_normal(part);
	if (poly->type != P_WALL)
	{
		int j = -1;
		while (++j < poly->vertices->len)
		{
			t_vec3 *vert = &poly->vertices->vertices[j];
			side = get_side_thin(part, (t_vec2){vert->x, vert->z});
			printf("%d // %f %f - %f %f\n", side, part.a.x, part.a.y, part.b.x, part.b.y);
			vert->y += line_get_distance(part, (t_vec2){vert->x, vert->z}) * (axis > 0 ? 1 : -1) * 0.05 * side;
		}
		j = -1;
		int len = floor(poly->indices->len / 3.);
		while (++j < len)
			poly->normals[j] = get_polygon_normal(poly);
	}
	else if (poly->type == P_WALL)
	{
		int len = floor(poly->indices->len / 3.);
		int j = -1;
		while (++j < len)
		{
			if (j % 2 == 0)
			{
				t_vec3 *vert = &poly->vertices->vertices[poly->indices->values[j * 3 + 1]];
				side = get_side_thin(part, (t_vec2){vert->x, vert->z});
				vert->y += line_get_distance(part, (t_vec2){vert->x, vert->z}) * (axis > 0 ? 1 : -1) * 0.05 * side;
			}
			else
			{
				t_vec3 *vert = &poly->vertices->vertices[poly->indices->values[j * 3]];
				side = get_side_thin(part, (t_vec2){vert->x, vert->z});
				vert->y += line_get_distance(part, (t_vec2){vert->x, vert->z}) * (axis > 0 ? 1 : -1) * 0.05 * side;
				vert = &poly->vertices->vertices[poly->indices->values[j * 3 + 1]];
				side = get_side_thin(part, (t_vec2){vert->x, vert->z});
				vert->y += line_get_distance(part, (t_vec2){vert->x, vert->z}) * (axis > 0 ? 1 : -1) * 0.05 * side;
				vert = &poly->vertices->vertices[poly->indices->values[j * 3 + 2]];
				side = get_side_thin(part, (t_vec2){vert->x, vert->z});
				vert->y += line_get_distance(part, (t_vec2){vert->x, vert->z}) * (axis > 0 ? 1 : -1) * 0.05 * side;
			}
		}
	}
}

void	modify_room(t_doom *doom, t_node *node, SDL_Event *event)
{
	int				i;
	t_polygon		poly;

	i = -1;
	while (++i < node->polygons->len)
	{
		poly = node->polygons->polygons[i];
		if (event->key.keysym.scancode == SDL_SCANCODE_PAGEUP 
			|| event->key.keysym.scancode == SDL_SCANCODE_PAGEDOWN)
			up_down_polygon(&poly, event->key.keysym.scancode == SDL_SCANCODE_PAGEUP);
		if (event->key.keysym.scancode == SDL_SCANCODE_T || event->key.keysym.scancode == SDL_SCANCODE_G)
			rotate_polygon(&poly, &doom->player, (event->key.keysym.scancode == SDL_SCANCODE_T ? 1 : -1));
		if (event->key.keysym.scancode == SDL_SCANCODE_F || event->key.keysym.scancode == SDL_SCANCODE_H)
			rotate_polygon(&poly, &doom->player, (event->key.keysym.scancode == SDL_SCANCODE_F ? 2 : -2));
	}
}

void	room_map(t_doom *doom, SDL_Event *event, void (*part)(t_doom *doom, t_node *node, SDL_Event *event))
{
	const t_node	*n = get_player_node(doom);
	t_node			*e;

	if (!n || n->parent->back == n)
		return ;
	part(doom, (t_node *)n, event);
	e = n->parent;
	while (e 
		&& (!e->parent 
		|| (e->parent->front == e 
			|| (e->parent->back == e && e->parent->back->type != N_LEAF))))
	{
		part(doom, e, event);
		if (e->parent && e->parent->back == e)
			break;
		e = e->parent;
	}
}

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (doom->guis[doom->current_gui].on_event != NULL)
		doom->guis[doom->current_gui].on_event(&doom->guis[doom->current_gui], event, doom);
	if (event->type == SDL_QUIT)
		doom->running = FALSE;
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
			doom->guis[doom->current_gui].components[i]
				->on_click(doom->guis[doom->current_gui].components[i],
				(t_vec2){ event->button.x, event->button.y }, doom);
	}
	if (event->type == SDL_MOUSEMOTION)
	{
		doom->mouse = (t_vec2){ event->motion.x, event->motion.y };
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
		{
			doom->guis[doom->current_gui].components[i]
				->on_mouse_move(doom->guis[doom->current_gui].components[i],
				doom->mouse, doom);
		}
	}
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_PAGEUP 
		|| key == SDL_SCANCODE_PAGEDOWN || key == SDL_SCANCODE_T 
		|| key == SDL_SCANCODE_G || key == SDL_SCANCODE_F 
		|| key == SDL_SCANCODE_H))
		room_map(doom, event, modify_room);
}

void	hook_events(t_doom *doom)
{

	const double	ms = doom->stats.delta * 3.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);
	SDL_Event		event;

	if (s[SDL_SCANCODE_ESCAPE])
		doom->running = FALSE;
	t_vec3 dir = {0, 0, 0};
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		dir.x += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
		dir.z += cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		dir.x += -cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
		dir.z += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_SPACE] || s[SDL_SCANCODE_LSHIFT])
		dir.y += (s[SDL_SCANCODE_SPACE] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
		doom->player.rotation.y += 0.3 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
		doom->player.rotation.x += 0.3 * (s[SDL_SCANCODE_I] ? 1 : -1) * ms;
	doom->player.pos = ft_vec3_add(doom->player.pos, dir);
	update_maxtrix(doom);
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	doom->player.curr_node = get_player_node(doom);
	SDL_PumpEvents();
}
