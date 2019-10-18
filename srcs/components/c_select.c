/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_select.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 22:53:11 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/19 01:43:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

void		c_select_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_select	*select;
	t_vec2		pos;
	int			height;
	int			curr_item;

	select = (t_select *)self;
	pos = (t_vec2){ event->motion.x, event->motion.y };
	curr_item = pos.y >= self->bounds.y + self->bounds.h ? (pos.y - (self->bounds.y + self->bounds.h)) / select->item_height : -1;
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		height = self->bounds.h + select->items->len * select->item_height;
		if (select->open)
		{
			if (!in_bounds(self->bounds, pos) 
				&& pos.x >= self->bounds.x && pos.y >= self->bounds.y
				&& pos.y < self->bounds.y + height
				&& pos.x < self->bounds.x + self->bounds.w)
				select->selected_item = curr_item;
			select->open = FALSE;
		}
		if (in_bounds(self->bounds, pos))
			select->open = TRUE;
	}
	if (event->type == SDL_MOUSEMOTION)
	{
		pos = (t_vec2){ event->motion.x, event->motion.y };
		select->hover_item = curr_item;
	}
}

void		c_select_render(t_doom *doom, t_component *self, t_img *image)
{
	t_select	*select;
	int			i;

	if (self->type != C_SELECT)
		return ;
	select = (t_select *)self;
	fill_rect(image, self->bounds, select->bg_color);
	if (select->selected_item != -1)
	{
		t_select_item	*item = &select->items->values[select->selected_item];
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
		item->name, (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(image, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){self->bounds.x, self->bounds.y + self->bounds.h / 2 - text->h / 2, text->w, text->h});
		SDL_FreeSurface(text);
	}
	if (select->open)
	{
		i = -1;
		while (++i < select->items->len)
		{
			fill_rect(image, (SDL_Rect){ self->bounds.x, self->bounds.y + self->bounds.h + i * select->item_height, self->bounds.w, select->item_height }, select->hover_item == i ? select->hover_color : select->bg_color);
			t_select_item	*item = &select->items->values[i];
			SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			item->name, (SDL_Color){255, 255, 255, 0});
			apply_surface_blended(image, text, (SDL_Rect){0, 0, text->w, text->h},
				(SDL_Rect){self->bounds.x, self->bounds.y + self->bounds.h + i * select->item_height, text->w, select->item_height});
			SDL_FreeSurface(text);
		}
	}
}

t_component	 *create_select(SDL_Rect bounds, char *text)
{
	t_select *select;

	if (!(select = (t_select *)ft_memalloc(sizeof(t_select))))
		return (NULL);
	select->super = (t_component) { .bounds = bounds, .type = C_SELECT, 
		.render = c_select_render, .on_event = c_select_on_event };
	select->bg_color = 0xFF505050;
	select->fg_color = 0xFFFFFFFF;
	select->selected_item = -1;
	select->text = text;
	select->item_height = 30;
	if (!(select->items = create_select_items_array(5)))
		return (NULL);
	return ((t_component *)select);
}