/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 08:21:09 by llelievr         ###   ########.fr       */
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
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_F2))
	{
		if (doom->fullscreen == TRUE)
		{
			doom->fullscreen = FALSE;
			SDL_SetWindowFullscreen(doom->win,0);
		}
		else
		{
			doom->fullscreen = TRUE;
			SDL_SetWindowFullscreen(doom->win,SDL_WINDOW_FULLSCREEN);
		}
	}
	
	return (TRUE);
}

void		hook_events(t_doom *doom)
{
	SDL_Event		event;
	const Uint8		*s = SDL_GetKeyboardState(NULL);

//	SDL_SetRelativeMouseMode((SDL_bool)doom->mouse_focus);
	doom->help = FALSE;
	if (s[SDL_SCANCODE_F1] && doom->current_gui != GUI_RESSOURCES) //securiser context ressource
		doom->help = TRUE;
	while (SDL_PollEvent(&event))
		if (!events_window(doom, &event))
			break;
	SDL_PumpEvents();
}
