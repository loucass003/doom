/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/18 19:25:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <libft.h>
#include "doom.h"
#include "gui.h"

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	gui_events(doom, doom->guis, event, doom->current_gui);
	components_events(doom, doom->guis, event, doom->current_gui);
	
	if (event->type == SDL_QUIT 
		|| (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_ESCAPE))
		doom->running = FALSE;
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_HOME))
		set_gui(doom, GUI_MAIN_MENU);
}

void	hook_events(t_doom *doom)
{
	SDL_Event		event;

//	SDL_SetRelativeMouseMode(doom->mouse_focus);
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}