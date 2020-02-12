/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_message.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:08:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/12 18:00:49 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"
#include "gui.h"
#include "script.h"

void			g_message_on_enter(t_gui *self, t_doom *doom)
{

}

void			g_message_on_leave(t_gui *self, t_doom *doom)
{
}

void			g_message_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

    if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_RETURN)
        doom->message.counter++;
}

void			g_message_render(t_gui *self, t_doom *doom)
{
    SDL_Surface		*text;
    
    if (doom->message.open == TRUE && doom->message.message_data && doom->message.message_data->textes_count > doom->message.counter)
    {
        fill_rect(&doom->screen, (SDL_Rect)
            { 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
        draw_rect(&doom->screen, (SDL_Rect)
            { 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
        
        text = TTF_RenderText_Blended(doom->fonts.helvetica, doom->message.message_data->textes[doom->message.counter], (SDL_Color){255, 255, 255, 0});
        apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
        SDL_FreeSurface(text);

        text = TTF_RenderText_Blended(doom->fonts.helvetica, "PRESS ENTER", (SDL_Color){255, 255, 255, 0});
        apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 200, S_HEIGHT - 50, text->w, text->h});
        SDL_FreeSurface(text);
        if (doom->message.message_data->textes_count == doom->message.counter)
            doom->message.open = FALSE;
    }
}


