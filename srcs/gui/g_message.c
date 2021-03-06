/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_message.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:08:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/18 01:17:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"
#include "gui.h"
#include "script.h"

void			g_message_on_event(t_gui *self, SDL_Event *e, t_doom *doom)
{
	const SDL_Scancode	k = e->key.keysym.scancode;
	t_message			*message;

	(void)self;
	message = &doom->message;
	if (e->type == SDL_KEYDOWN && k == SDL_SCANCODE_RETURN)
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
	if (e->type == SDL_KEYDOWN && (k == SDL_SCANCODE_1 || k == SDL_SCANCODE_2))
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
