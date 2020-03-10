/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_message.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:08:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/10 21:32:59 by llelievr         ###   ########.fr       */
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
	{
		if (doom->message.message_data && doom->message.counter_m != -1)
		{
			doom->message.counter_m++;
			if (doom->message.message_data->textes_count == doom->message.counter_m)
			{
				doom->message.counter_m = -1;
				doom->message.counter_q = -1;
			}
		}
		else if (doom->message.question_data)
		{
			doom->message.counter_q++;
			if (doom->message.question_data->answer_count == doom->message.counter_q)
				doom->message.open = FALSE;
		}
	}
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_1 || key == SDL_SCANCODE_2))
	{
		doom->message.counter_q = 0;
	}
}

void			g_message_render(t_gui *self, t_doom *doom)
{
	SDL_Surface		*text;
	
	if (doom->message.open == TRUE && doom->message.message_data && doom->message.message_data->textes_count > doom->message.counter_m && doom->message.counter_m != -1)
	{
		fill_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
		draw_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
		
		text = TTF_RenderText_Blended(doom->fonts.helvetica, doom->message.message_data->textes[doom->message.counter_m], (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
		SDL_FreeSurface(text);

		text = TTF_RenderText_Blended(doom->fonts.helvetica, "PRESS ENTER", (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 200, S_HEIGHT - 50, text->w, text->h});
		SDL_FreeSurface(text);
	}
	else if (doom->message.open == TRUE && doom->message.question_data)
	{
		fill_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
		draw_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
		if (doom->message.question_data->quest_count == 2 && doom->message.counter_q == -1 && doom->message.counter_m == -1)
		{
			int h = 100;
			text = TTF_RenderText_Blended(doom->fonts.helvetica, doom->message.question_data->quest[0], (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - h, text->w, text->h});
			SDL_FreeSurface(text);
			h = 50 ;
			text = TTF_RenderText_Blended(doom->fonts.helvetica, doom->message.question_data->quest[1], (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - h, text->w, text->h});
			SDL_FreeSurface(text);

			text = TTF_RenderText_Blended(doom->fonts.helvetica, "CHOOSE AN ANSWER", (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - (text->w + 10), S_HEIGHT - 50, text->w, text->h});
			SDL_FreeSurface(text);
		}
		else if (doom->message.question_data->answer_count > doom->message.counter_q && doom->message.counter_q != -1)
		{
			text = TTF_RenderText_Blended(doom->fonts.helvetica, doom->message.question_data->answer[doom->message.counter_q], (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH/2 - text->w/2, S_HEIGHT - 100, text->w, text->h});
			SDL_FreeSurface(text);

			text = TTF_RenderText_Blended(doom->fonts.helvetica, "PRESS ENTER", (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 200, S_HEIGHT - 50, text->w, text->h});
			SDL_FreeSurface(text);
		}
	}
}
