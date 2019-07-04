/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/28 22:43:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_node	*get_player_node(t_doom *doom)
{
	t_node *n = doom->bsp;
	t_vec2 p = (t_vec2){doom->player.pos.x, doom->player.pos.z};
	while (n->type != N_LEAF)
	{
		if (get_side_thin(n->partition, p) == S_FRONT)
			n = n->front;
		else
			n = n->back;
	}
	return (n);
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
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
		{
			doom->guis[doom->current_gui].components[i]
				->on_mouse_move(doom->guis[doom->current_gui].components[i],
				(t_vec2){ event->motion.x, event->motion.y }, doom);
		}
	}
	if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_PAGEUP)
	{
		t_node *n = get_player_node(doom);
		int i = -1;
		while (++i < n->polygons->len)
		{
			t_polygon poly = n->polygons->polygons[i];
			if (poly.type == P_FLOOR)
				continue;
			int j = -1;
			while (++j < poly.vertices->len)
			{
				poly.vertices->vertices[j].y += 0.05;
			}
			t_node *wall = n->parent;
			while (wall && (!wall->parent || wall->parent->front == wall))
			{
				int k = -1;
				while (++k < wall->polygons->len)
				{
					t_polygon poly2 = wall->polygons->polygons[k];
					if (poly2.type != P_WALL)
						continue;
					int l = -1;
					int len = floor(poly2.indices->len / 3.);
					printf("%d\n", len);
					while (++l < len)
					{
						if (l % 2 == 0)
							poly2.vertices->vertices[poly2.indices->values[l * 3]].y += 0.05;
						else
						{
							poly2.vertices->vertices[poly2.indices->values[l * 3 + 1]].y += 0.05;
						}
					}
				}
				wall = wall->parent;
			}
		}
	}
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
		dir.x += sinf(doom->player.rotation) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
		dir.z += cosf(doom->player.rotation) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		dir.x += -cosf(doom->player.rotation) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
		dir.z += sinf(doom->player.rotation) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_SPACE] || s[SDL_SCANCODE_LSHIFT])
		dir.y += (s[SDL_SCANCODE_SPACE] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
		doom->player.rotation += 0.3 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
	doom->player.pos = ft_vec3_add(doom->player.pos, dir);
	update_maxtrix(doom);
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	doom->player.curr_node = get_player_node(doom);
	SDL_PumpEvents();
}
