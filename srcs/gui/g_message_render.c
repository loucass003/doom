/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_message_render.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 22:01:25 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 01:48:43 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "script.h"
#include "editor.h"

void			g_message_render_press_enter(t_doom *doom)
{
	SDL_Surface		*t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica, "PRESS ENTER",
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t,
		(SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - 200, S_HEIGHT - 50, t->w, t->h});
	SDL_FreeSurface(t);
}

void			g_message_render_message(t_doom *doom, t_action_message *md)
{
	SDL_Surface		*t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica,
		md->textes[doom->message.counter_m], (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t,
		(SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 100, t->w, t->h});
	SDL_FreeSurface(t);
	g_message_render_press_enter(doom);
}

void			g_message_render_question(t_doom *doom, t_action_question *q)
{
	SDL_Surface		*t;

	t = TTF_RenderText_Blended(doom->fonts.helvetica, q->quest[0],
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t,
		(SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 100, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, q->quest[1],
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t,
		(SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 40, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "CHOOSE AN ANSWER",
		(SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, t,
		(SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH - (t->w + 10), S_HEIGHT - 50, t->w, t->h});
	SDL_FreeSurface(t);
}

void			g_message_render_questions(t_doom *doom, t_action_question *q)
{
	SDL_Surface *t;

	if (q->quest_count == 2 && doom->message.counter_q == -1
		&& doom->message.counter_m == -1)
		g_message_render_question(doom, q);
	else if (q->answer_count > doom->message.counter_q
		&& doom->message.counter_q != -1)
	{
		t = TTF_RenderText_Blended(doom->fonts.helvetica,
			q->answer[doom->message.counter_q], (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, t,
			(SDL_Rect){0, 0, t->w, t->h},
			(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 100, t->w, t->h});
		SDL_FreeSurface(t);
		g_message_render_press_enter(doom);
	}
}
