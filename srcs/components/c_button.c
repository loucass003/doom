/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_button.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:59:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 12:54:16 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

void		c_button_render(t_doom *doom, t_component *self, t_img *image)
{
	t_button	*btn;
	int			x;
	int			y;

	(void)image;
	if (self->type != C_BUTTON)
		return ;
	btn = (t_button *)self;
	btn->color = btn->selected ? 0xFFFFFFFF : btn->color;
	x = self->bounds.x;
	while (x < self->bounds.x + self->bounds.w)
	{
		y = self->bounds.y;
		while (y < self->bounds.y + self->bounds.h)
		{
			doom->screen.pixels[(y * doom->screen.width) + x] = btn->color;
			y++;
		}
		x++;
	}
	if (btn->image)
		apply_btn_image(doom, self, btn);
	if (btn->texte != NULL)
		apply_text(doom, self, btn);
}

t_bool		c_button_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_button	*btn;

	btn = (t_button *)self;
	if (in_bounds(self->bounds, (t_vec2){event->motion.x, event->motion.y}))
		btn->color = btn->color_hover;
	else
		btn->color = btn->color_default;
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (in_bounds(self->bounds, (t_vec2){event->motion.x, event->motion.y})
			&& self->perform_action != NULL)
			if (!self->perform_action(self, doom))
				return (FALSE);
	}
	return (TRUE);
}

t_component	*create_button(SDL_Rect bounds, char *s, char *s2)
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
	btn->color = btn->color_default;
	btn->selected = FALSE;
	btn->image = s;
	btn->texte = s2;
	return ((t_component *)btn);
}
