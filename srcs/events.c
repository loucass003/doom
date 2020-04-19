/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 18:05:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <libft.h>
#include "doom.h"
#include "gui.h"
#include "script.h"

static t_bool	events_full_screen(t_doom *doom, const SDL_Scancode key,
	SDL_Event *event)
{
	(void)key;
	(void)event;
	if (doom->fullscreen == TRUE)
	{
		doom->fullscreen = FALSE;
		SDL_SetWindowFullscreen(doom->win, 0);
	}
	else
	{
		doom->fullscreen = TRUE;
		SDL_SetWindowFullscreen(doom->win, SDL_WINDOW_FULLSCREEN);
	}
	return (TRUE);
}

t_bool			key_events(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (event->type == SDL_QUIT
		|| (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_ESCAPE))
	{
		set_gui(doom, GUI_ESC);
		doom->running = FALSE;
		SDL_SetRelativeMouseMode((SDL_bool)FALSE);
		return (FALSE);
	}
	if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_H)
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_F2))
		events_full_screen(doom, key, event);
	if (event->type == SDL_USEREVENT && event->user.type == doom->trigger_event)
	{
		trigger_script(doom, *((t_trigger *)event->user.data1));
		free(event->user.data1);
	}
	return (TRUE);
}

static t_bool	events_window(t_doom *doom, SDL_Event *event)
{
	gui_events(doom, doom->guis, event, doom->current_gui);
	components_events(doom, doom->guis, event, doom->current_gui);
	if (event->type == SDL_DROPFILE)
		SDL_free(event->drop.file);
	if (!key_events(doom, event))
		return (FALSE);
	return (TRUE);
}

void			hook_events(t_doom *doom)
{
	SDL_Event		event;
	const Uint8		*s = SDL_GetKeyboardState(NULL);

	SDL_SetRelativeMouseMode((SDL_bool)doom->mouse_focus);
	doom->help = FALSE;
	if (s[SDL_SCANCODE_F1] && doom->current_gui != GUI_RESSOURCES)
		doom->help = TRUE;
	while (SDL_PollEvent(&event))
		if (!events_window(doom, &event))
			break ;
	SDL_PumpEvents();
}
