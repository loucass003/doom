/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/23 14:04:19 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <libft.h>
#include "doom.h"
#include "gui.h"

static t_bool	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	gui_events(doom, doom->guis, event, doom->current_gui);
	components_events(doom, doom->guis, event, doom->current_gui);
	if (event->type == SDL_QUIT
		|| (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_ESCAPE))
	{
		set_gui(doom, GUI_ESC);
		doom->running = FALSE;
		return (FALSE);
	}
	if (event->type == SDL_KEYDOWN && ((key == SDL_SCANCODE_HOME)
		|| (key == SDL_SCANCODE_H)))
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
	return (TRUE);
}

void		hook_events(t_doom *doom)
{
	SDL_Event		event;

	SDL_SetRelativeMouseMode((SDL_bool)doom->mouse_focus);
	while (SDL_PollEvent(&event))
		if (!events_window(doom, &event))
			break;
	SDL_PumpEvents();
}
