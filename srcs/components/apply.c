/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:33:32 by louali            #+#    #+#             */
/*   Updated: 2020/02/04 15:34:50 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void		apply_btn_image(t_doom *doom, t_component *self, t_button *btn)
{
	const t_vec2		size = ft_vec2_sub(btn->img_end, btn->img_start);

	apply_image_to_image(&doom->screen, btn->img,
		(SDL_Rect) {btn->img_start.x, btn->img_start.y, size.x + 2, size.y},
		(SDL_Rect) {self->bounds.x + 2, self->bounds.y +
		2, self->bounds.w - 4, self->bounds.h - 4});
}

void		apply_text(t_doom *doom, t_component *self, t_button *btn)
{
	SDL_Surface *text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		btn->texte, btn->colortext);
	apply_surface_blended(&doom->screen, text,
		(SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){self->bounds.x + self->bounds.w / 2 - text->w / 2,
		self->bounds.y + self->bounds.h / 2 - text->h / 2,
		text->w, text->h});
	SDL_FreeSurface(text);
}

void		apply_textfield(t_doom *doom, char *str,
	t_component *self, t_textfield *tf)
{
	int				x;
	int				y;
	SDL_Surface		*text;
	t_color			c;
	int				offset_x;

	TTF_SizeText(doom->fonts.helvetica, str, &x, &y);
	offset_x = ft_strlen(str) > (self->bounds.w / (x / ft_strlen(str)))
	? ft_strlen(str) - (self->bounds.w / (x / ft_strlen(str))) : 0;
	c = ft_i_color(tf->fg_color);
	text = TTF_RenderText_Blended(doom->fonts.helvetica, str
		+ offset_x, (SDL_Color){c.r, c.g, c.b, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w,
		text->h}, (SDL_Rect){self->bounds.x, self->bounds.y
		+ self->bounds.h / 2 - text->h / 2, text->w, text->h - 2});
	if (tf->bar && tf->focus)
		draw_line(&doom->screen, (t_pixel){self->bounds.x + 2
			+ fmin(self->bounds.w - 12, text->w), self->bounds.y
			+ self->bounds.h - 5, 0xFFFFFFFF}, (t_pixel){self->bounds.x
			+ fmin(self->bounds.w - 12, text->w) + 12,
			self->bounds.y + self->bounds.h - 5, 0});
	SDL_FreeSurface(text);
}
