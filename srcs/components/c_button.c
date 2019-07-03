/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_button.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:59:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/03 14:18:33 by lloncham         ###   ########.fr       */
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
		{
			if (btn->image)
			{
				if (doom->editor.curseur == 1 && ft_strcmp(btn->image, "icons/curseur3.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				if (doom->editor.poly == 1 && ft_strcmp(btn->image, "icons/poly.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				if (doom->editor.sup != 0 && ft_strcmp(btn->image, "icons/icons8-effacer-96.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				else if (doom->editor.set_start == 1 && ft_strcmp(btn->image, "icons/pin.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				else if (doom->editor.lignes == 1 && ft_strcmp(btn->image, "icons/icons8-stylo-64.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				else if (doom->editor.porte == 1 && ft_strcmp(btn->image, "icons/icons8-porte-64.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
				else if (doom->editor.secteur == 1 && ft_strcmp(btn->image, "icons/icons8-modifier-la-ligne-80.png") == 0)
					doom->screen.pixels[(y * doom->screen.width) + x] = 858585;
			}
			else
				doom->screen.pixels[(y * doom->screen.width) + x] = btn->color;	
		}
	if (btn->image)
	{
		t_texture *tmp;

		tmp = doom->icons;
		while (tmp != NULL && ft_strcmp(btn->image, tmp->name) != 0)
			tmp = tmp->next;
		if (ft_strcmp(btn->image, tmp->name) == 0)
			apply_surface(&doom->screen, tmp->text,
			(SDL_Rect) {0, 0, tmp->text->w, tmp->text->h},
			(SDL_Rect) {self->bounds.x + 2, self->bounds.y +
			2, self->bounds.w - 4, self->bounds.h - 4});
	}
}

void		c_button_on_click(t_component *self, t_vec2 pos, t_doom *doom)
{
	if (in_bounds(self->bounds, pos) && self->perform_action != NULL)
		self->perform_action(self, doom);
}

void		c_button_on_mouse_move(t_component *self, t_vec2 pos, t_doom *doom)
{
	t_button	*btn;

	btn = (t_component*)self;
	if (in_bounds(self->bounds, pos))
		btn->color = btn->color_hover;
	else
		btn->color = btn->color_default;
}

t_component	 *create_button(SDL_Rect bounds, char *s)
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
	btn->image = s;
	return ((t_component *)btn);
}
