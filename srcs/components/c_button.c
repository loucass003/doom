/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_button.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:59:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 15:00:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		c_button_render(t_doom *doom, t_component *self)
{
	t_button	*btn;

	if (self->type != C_BUTTON)
		return ;
	btn = (t_button *)self;
	for (int x = self->bounds.x; x < self->bounds.x + self->bounds.w; x++)
		for (int y = self->bounds.y; y < self->bounds.y + self->bounds.h; y++)
			doom->screen.pixels[(y * doom->screen.width) + x] = btn->color;
}

void		c_button_on_click(t_component *self, t_vec2 pos, t_doom *doom)
{
	if (pos.x >= self->bounds.x && pos.y >= self->bounds.y 
		&& pos.y < self->bounds.y + self->bounds.h 
		&& pos.x < self->bounds.x + self->bounds.w
		&& self->perform_action != NULL)
		self->perform_action(self, doom);
}

void		c_button_on_mouse_move(t_component *self, t_vec2 pos, t_doom *doom)
{
	t_button	*btn;

	btn = (t_component*)self;
	if (pos.x >= self->bounds.x && pos.y >= self->bounds.y 
		&& pos.y < self->bounds.y + self->bounds.h 
		&& pos.x < self->bounds.x + self->bounds.w)
			btn->color = btn->color_hover;
	else
		btn->color = btn->color_default;
}

t_component	 *create_button(SDL_Rect bounds)
{
	t_button *btn;

	if (!(btn = (t_button *)malloc(sizeof(t_button))))
		return (NULL);
	ft_bzero(btn, sizeof(t_button));
	btn->super = (t_component) { .bounds = bounds, .type = C_BUTTON, 
		.render = c_button_render, .on_click = c_button_on_click,
		.on_mouse_move = c_button_on_mouse_move};
	btn->color_default = 0xFFFFFF;
	btn->color_hover = 0xCCCCCC;
	btn->color = btn->color_default;
	return ((t_component *)btn);
}

