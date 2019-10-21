/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_textfield.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 03:16:01 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/21 05:25:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

t_bool		c_textfield_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_textfield	*tf;

	tf = (t_textfield *)self;
	if (tf->focus && event->type == SDL_TEXTINPUT)
	{
		if (tf->text_len < 255)
		{
			ft_strcat(tf->text, event->text.text);
			tf->text_len++;
		}
	}
	else if (tf->focus && event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
	{
		if (tf->text_len > 0)
			tf->text[--tf->text_len] = 0;
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
		tf->focus = in_bounds(self->bounds, (t_vec2){ event->motion.x, event->motion.y });
	return (TRUE);
}

void		c_textfield_render(t_doom *doom, t_component *self, t_img *image)
{
	static double	t0 = 0;
	t_textfield		*tf;
	char			*str;

	if (self->type != C_TEXTFIELD)
		return ;
	t0 += 3 * doom->stats.delta;
	tf = (t_textfield *)self;
	if (t0 > 1 && !(t0 = 0))
		tf->bar = !tf->bar;
	fill_rect(image, self->bounds, tf->bg_color);
	str = tf->text_len == 0 && !tf->focus && tf->placeholder ? tf->placeholder : tf->text;
	if (str && *str)
	{
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
		str, (SDL_Color){255, 255, 255, 0});
		int  char_w = text->w / tf->text_len;
		int offset_x = 0;
		offset_x = (text->w / self->bounds.w);
		apply_surface_blended(image, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){self->bounds.x + offset_x, self->bounds.y + self->bounds.h / 2 - text->h / 2, fmin(self->bounds.w, text->w), text->h});
		if (tf->bar && tf->focus)
			draw_line(image, (t_pixel){ self->bounds.x + 2 + text->w , self->bounds.y + self->bounds.h - 5, 0xFFFFFFFF},  (t_pixel){ self->bounds.x + text->w + 12, self->bounds.y + self->bounds.h - 5, 0});
		SDL_FreeSurface(text);
	}
	else if (tf->bar && tf->focus)
			draw_line(image, (t_pixel){ self->bounds.x + 2, self->bounds.y + self->bounds.h - 5, 0xFFFFFFFF},  (t_pixel){ self->bounds.x + 12, self->bounds.y + self->bounds.h - 5, 0});
}

t_component	 *create_textfield(SDL_Rect bounds, char *placeholder)
{
	t_textfield	*tf;

	if (!(tf = (t_textfield *)ft_memalloc(sizeof(t_textfield))))
		return (NULL);
	tf->super = (t_component) { .visible = TRUE, .bounds = bounds, .type = C_TEXTFIELD, 
		.render = c_textfield_render, .on_event = c_textfield_on_event };
	if (!(tf->text = ft_memalloc(sizeof(char) * 255)))
		return (NULL);
	tf->placeholder = placeholder;
	tf->bg_color = 0xFF505050;
	tf->fg_color = 0xFFFFFFFF;
	return ((t_component *)tf);
}