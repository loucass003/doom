/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_checkbox.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 01:21:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/14 04:49:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void		c_checkbox_render(t_doom *doom, t_component *self, t_img *img)
{
	t_checkbox	*checkbox;

	if (self->type != C_CHECKBOX)
		return ;
	checkbox = (t_checkbox *)self;
	fill_rect(img, (SDL_Rect){ self->bounds.x, self->bounds.y, 20, 20 },
		checkbox->bg_color);
	SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
		checkbox->text, (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){self->bounds.x + 25, self->bounds.y + self->bounds.h / 2 - 20 / 2, self->bounds.w - 25, self->bounds.h});
	SDL_FreeSurface(text);
	if (checkbox->value)
	{
		draw_line(img, (t_pixel){ self->bounds.x, self->bounds.y, checkbox->fg_color }, (t_pixel){ self->bounds.x + 20, self->bounds.y + 20, checkbox->fg_color });
		draw_line(img, (t_pixel){ self->bounds.x, self->bounds.y + 20, checkbox->fg_color }, (t_pixel){ self->bounds.x + 20, self->bounds.y, checkbox->fg_color });
		draw_rect(img, (SDL_Rect){ self->bounds.x, self->bounds.y, 20, 20 }, 0xFFFFFFFF);
	}
}

t_bool		c_checkbox_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_checkbox	*checkbox;

	checkbox = (t_checkbox *)self;
	
	if (in_bounds(self->bounds, (t_vec2){ event->motion.x, event->motion.y }))
		checkbox->bg_color = checkbox->color_hover;
	else
		checkbox->bg_color = checkbox->color_default;

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (in_bounds(self->bounds, (t_vec2){ event->motion.x, event->motion.y }))
		{
			checkbox->value = !checkbox->value;
			if (self->perform_action != NULL && !self->perform_action(self, doom))
				return (FALSE);
		}
	}
	return (TRUE);
}

t_component	 *create_checkbox(t_doom *doom, t_vec2 pos, char *text)
{
	t_checkbox	*checkbox;
	SDL_Rect	bounds;
	int			x;

	if (!(checkbox = (t_checkbox *)malloc(sizeof(t_checkbox))))
		return (NULL);
	ft_bzero(checkbox, sizeof(t_checkbox));
	TTF_SizeText(doom->fonts.helvetica, text, &x, NULL);
	bounds.x = pos.x;
	bounds.y = pos.y - 10;
	bounds.w = x * 0.75 + 25;
	bounds.h = 20;
	checkbox->super = (t_component) { .enabled = TRUE, .visible = TRUE, 
		.bounds = bounds, .type = C_CHECKBOX, 
		.render = c_checkbox_render, .on_event = c_checkbox_on_event };
	//checkbox->bg_color = 0xFF505050;
	checkbox->color_default = 0xFF505050;
	checkbox->color_hover = 0xFF606060;
	checkbox->fg_color = 0xFFFFFFFF;
	checkbox->text = text;
	return ((t_component *)checkbox);
}
