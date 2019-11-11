/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_button.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:59:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/11 00:56:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void		c_button_render(t_doom *doom, t_component *self, t_img *image)
{
	t_button	*btn;

	if (self->type != C_BUTTON)
		return ;
	btn = (t_button *)self;
	btn->color = btn->selected ? 0xFFFFFFFF : btn->color;
	for (int x = self->bounds.x; x < self->bounds.x + self->bounds.w; x++)
		for (int y = self->bounds.y; y < self->bounds.y + self->bounds.h; y++)
			doom->screen.pixels[(y * doom->screen.width) + x] = btn->color;
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
	if (btn->texte != NULL)
	{
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			btn->texte, (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){self->bounds.x + self->bounds.w / 2 - text->w / 2, self->bounds.y + self->bounds.h / 2 - text->h / 2, text->w, text->h});
		SDL_FreeSurface(text);
	}
}

t_bool		c_button_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_button	*btn;

	btn = (t_button *)self;
	
	if (in_bounds(self->bounds, (t_vec2){ event->motion.x, event->motion.y }))
		btn->color = btn->color_hover;
	else
		btn->color = btn->color_default;

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (in_bounds(self->bounds, (t_vec2){ event->motion.x, event->motion.y }) 
			&& self->perform_action != NULL)
			if (!self->perform_action(self, doom))
				return (FALSE);
	}
	return (TRUE);
}

t_component	 *create_button(SDL_Rect bounds, char *s, char *s2)
{
	t_button *btn;

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
