/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/18 19:35:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (event->type == SDL_QUIT)
		doom->running = FALSE;
}

void	hook_events(t_doom *doom)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);
	SDL_Event		event;

	if (state[SDL_SCANCODE_ESCAPE])
		doom->running = FALSE;
	t_vec2 dir = {0, 0};
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])
		dir.y += (state[SDL_SCANCODE_W] ? -1 : 1);
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
		dir.x += (state[SDL_SCANCODE_D] ? -1 : 1);
	if (state[SDL_SCANCODE_J] || state[SDL_SCANCODE_L])
		doom->player.rotation += 0.01 * (state[SDL_SCANCODE_J] ? 1 : -1);
	doom->player.pos = ft_vec2_add(doom->player.pos, ft_mat2_mulv(ft_mat2_rotation(doom->player.rotation + M_PI_2), dir));
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
