/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_message.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:08:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 17:05:47 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"
#include "gui.h"
#include "script.h"

void			g_message_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	(void)self;
	const SDL_Scancode	key = event->key.keysym.scancode;
	t_message			*message;

	message = &doom->message;
	if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_RETURN)
	{
		if (message->message_data && message->counter_m != -1)
		{
			message->counter_m++;
			if (message->message_data->textes_count == message->counter_m)
			{
				message->counter_m = -1;
				message->counter_q = -1;
			}
		}
		else if (message->question_data)
		{
			message->counter_q++;
			if (message->question_data->answer_count == message->counter_q)
				message->open = FALSE;
		}
	}
	if (event->type == SDL_KEYDOWN
		&& (key == SDL_SCANCODE_1 || key == SDL_SCANCODE_2))
		message->counter_q = 0;
}

void			g_message_render(t_gui *self, t_doom *doom)
{
	(void)self;
	if (!doom->message.open)
		return ;
	if (doom->message.message_data
		&& doom->message.message_data->textes_count > doom->message.counter_m
		&& doom->message.counter_m != -1)
	{
		fill_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
		draw_rect(&doom->screen, (SDL_Rect)
				{ 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
		g_message_render_message(doom, doom->message.message_data);
	}
	else if (doom->message.question_data)
	{
		fill_rect(&doom->screen, (SDL_Rect)
			{ 0, S_HEIGHT - 150, S_WIDTH, 150 }, 0xFF303030);
		draw_rect(&doom->screen, (SDL_Rect)
				{ 0, S_HEIGHT - 150, S_WIDTH, 250 }, 0xFFFFFFFF);
		g_message_render_questions(doom, doom->message.question_data);
	}
}
