/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_textfield.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 03:16:01 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/06 14:04:03 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

t_bool		event_backspace(t_textfield *tf, t_component *self, t_doom *doom)
{
	if (tf->number && tf->focus)
	{
		tf->error = FALSE;
		tf->value = ft_atoi2(tf->text, &tf->error);
	}
	if (self->perform_action && !self->perform_action(self, doom))
		return (FALSE);
	return (TRUE);
}

t_bool		c_textfield_on_event(t_component *self,
SDL_Event *event, t_doom *doom)
{
	t_textfield	*tf;

	tf = (t_textfield *)self;
	if (tf->focus && event->type == SDL_TEXTINPUT)
	{
		if (tf->text_len++ < 255)
			ft_strcat(tf->text, event->text.text);
	}
	else if (tf->focus && event->type == SDL_KEYDOWN
		&& event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
	{
		if (tf->text_len > 0)
			tf->text[--tf->text_len] = 0;
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
		tf->focus = in_bounds(self->bounds,
			(t_vec2){ event->motion.x, event->motion.y });
	if ((event->type == SDL_KEYDOWN && event->key.keysym.scancode
		== SDL_SCANCODE_BACKSPACE) || event->type == SDL_TEXTINPUT)
		if (event_backspace(tf, self, doom) == FALSE)
			return (FALSE);
	return (TRUE);
}

void		c_textfield_render(t_doom *doom, t_component *self, t_img *image)
{
	t_textfield		*tf;
	char			*str;

	if (self->type != C_TEXTFIELD)
		return ;
	tf = (t_textfield *)self;
	tf->t0 += 3 * doom->stats.delta;
	if (tf->t0 > 1 && !(tf->t0 = 0))
		tf->bar = !tf->bar;
	fill_rect(image, self->bounds, tf->bg_color);
	if (tf->error)
		draw_rect(image, self->bounds, 0xFFFF0000);
	str = tf->text_len == 0 && !tf->focus
	&& tf->placeholder ? tf->placeholder : tf->text;
	if (str && *str)
		apply_textfield(doom, str, self, tf);
	else if (tf->bar && tf->focus)
		draw_line(image, (t_pixel){self->bounds.x + 2, self->bounds.y
			+ self->bounds.h - 5, 0xFFFFFFFF}, (t_pixel){self->bounds.x
			+ 12, self->bounds.y + self->bounds.h - 5, 0});
}

void		set_text_value(t_textfield *tf, char *text, int len)
{
	ft_memcpy(tf->text, text, len);
	ft_memset(tf->text + len, 0, 255 - len);
	tf->text_len = len;
}

t_component	*create_textfield(SDL_Rect bounds, char *placeholder, t_bool number)
{
	t_textfield		*tf;

	if (!(tf = (t_textfield *)ft_memalloc(sizeof(t_textfield))))
		return (NULL);
	tf->super = (t_component) { .enabled = TRUE, .visible = TRUE,
		.bounds = bounds, .type = C_TEXTFIELD,
		.render = c_textfield_render, .on_event = c_textfield_on_event };
	if (!(tf->text = ft_memalloc(sizeof(char) * 256)))
		return (NULL);
	tf->number = number;
	tf->placeholder = placeholder;
	tf->bg_color = 0xFF505050;
	tf->fg_color = 0xFFFFFFFF;
	return ((t_component *)tf);
}
