/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_button.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:59:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/06 13:45:01 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

void		set_button_cell(t_button *btn, int x, int y)
{
	t_vec2		div;
	t_vec2		pos;

	if (!btn->img)
		return ;
	pos = (t_vec2){ x, y };
	div = (t_vec2){ btn->img->width / btn->cells_count.x,
		btn->img->height / btn->cells_count.y };
	btn->img_start = ft_vec2_mul(pos, div);
	btn->img_end = ft_vec2_mul(ft_vec2_add(pos, (t_vec2){ 1, 1 }), div);
}

void		c_button_render(t_doom *doom, t_component *self, t_img *image)
{
	t_button	*btn;
	int			x;
	int			y;

	(void)image;
	if (self->type != C_BUTTON)
		return ;
	btn = (t_button *)self;
	btn->color = btn->hover ? btn->color_hover : btn->color_default;
	btn->color = self->enabled ? btn->color : 0xFF252525;
	btn->color = btn->selected ? 0xFFFFFFFF : btn->color;
	x = self->bounds.x;
	while (x++ < self->bounds.x + self->bounds.w)
	{
		y = self->bounds.y;
		while (y++ < self->bounds.y + self->bounds.h)
			doom->screen.pixels[(y * doom->screen.width) + x] = btn->color;
	}
	if (btn->img)
		apply_btn_image(doom, self, btn);
	btn->texte != NULL ? apply_text(doom, self, btn) : 0;
}

t_bool		c_button_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_button	*btn;

	btn = (t_button *)self;
	if (event->type == SDL_MOUSEMOTION)
	{
		btn->hover = FALSE;
		if (in_bounds(self->bounds, (t_vec2){event->motion.x, event->motion.y}))
			btn->hover = TRUE;
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (in_bounds(self->bounds, (t_vec2){event->motion.x, event->motion.y})
			&& self->perform_action != NULL)
			if (!self->perform_action(self, doom))
				return (FALSE);
	}
	return (TRUE);
}

t_component	*create_button(SDL_Rect bounds, t_img *img, char *s2)
{
	t_button	*btn;

	if (!(btn = (t_button *)malloc(sizeof(t_button))))
		return (NULL);
	ft_bzero(btn, sizeof(t_button));
	btn->super = (t_component) { .enabled = TRUE, .bounds = bounds,
		.visible = TRUE, .type = C_BUTTON, .render = c_button_render,
		.on_event = c_button_on_event};
	btn->color_default = 0xFF505050;
	btn->color_hover = 0xFF606060;
	btn->colortext = (SDL_Color) {255, 255, 255, 0};
	btn->color = btn->color_default;
	btn->selected = FALSE;
	btn->img = img;
	btn->texte = s2;
	btn->cells_count = (t_vec2){ 1, 1 };
	set_button_cell(btn, 0, 0);
	return ((t_component *)btn);
}
