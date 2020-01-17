/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_select.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 22:53:11 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 17:36:25 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

static void	mouseup(t_component *self, t_vec2 pos, t_doom *doom, int it)
{
	int			height;
	t_select	*select;

	select = (t_select *)self;
	height = self->bounds.h + (select->items ? select->items->len : 0)
	* select->item_height;
	if (!in_bounds(self->bounds, pos)
		&& pos.x >= self->bounds.x && pos.y >= self->bounds.y
		&& pos.y < self->bounds.y + height
		&& pos.x < self->bounds.x + self->bounds.w)
	{
		select->selected_item = it;
		if (self->perform_action && !self->perform_action(self, doom))
			return ;
	}
	select->open = FALSE;
}

t_bool		c_select_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_select	*select;
	t_vec2		pos;
	int			curr_item;

	select = (t_select *)self;
	pos = (t_vec2){ event->motion.x, event->motion.y };
	curr_item = pos.y >= self->bounds.y + self->bounds.h ?
		(pos.y - (self->bounds.y + self->bounds.h)) / select->item_height : -1;
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (select->open)
		{
			mouseup(self, pos, doom, curr_item);
			return (FALSE);
		}
		if (in_bounds(self->bounds, pos))
			select->open = TRUE;
	}
	else if (event->type == SDL_MOUSEMOTION)
		select->hover_item = curr_item;
	return (TRUE);
}

void		opens(t_select *select, t_img *img, t_component *self, t_doom *doom)
{
	int				i;
	t_select_item	*item;
	SDL_Surface		*text;

	i = -1;
	while (++i < select->items->len)
	{
		fill_rect(img, (SDL_Rect){ self->bounds.x,
			self->bounds.y + self->bounds.h + i * select->item_height,
			self->bounds.w, select->item_height },
			select->hover_item == i ? select->hover_color
			: select->bg_color);
		item = &select->items->values[i];
		text = TTF_RenderText_Blended(doom->fonts.helvetica,
			item->name, (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(img, text,
			(SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){self->bounds.x, self->bounds.y + self->bounds.h + i
			* select->item_height, text->w, select->item_height});
		SDL_FreeSurface(text);
	}
}

void		c_select_render(t_doom *doom, t_component *self, t_img *image)
{
	t_select		*select;
	SDL_Surface		*text;
	t_select_item	*it;
	t_color			c;

	if (self->type != C_SELECT)
		return ;
	select = (t_select *)self;
	fill_rect(image, self->bounds, select->bg_color);
	it = select->selected_item == -1 ? NULL :
		&select->items->values[select->selected_item];
	c = ft_i_color(select->fg_color);
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
	it ? it->name : select->text, (SDL_Color){c.r, c.g, c.b, 0});
	apply_surface_blended(image, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){self->bounds.x, self->bounds.y + self->bounds.h / 2
		- text->h / 2, fmin(self->bounds.w, text->w), text->h});
	SDL_FreeSurface(text);
	if (select->open && select->items)
		opens(select, image, self, doom);
}

t_component	*create_select(SDL_Rect bounds, char *text)
{
	t_select *select;

	if (!(select = (t_select *)ft_memalloc(sizeof(t_select))))
		return (NULL);
	select->super = (t_component) { .enabled = TRUE, .visible = TRUE,
		.bounds = bounds, .type = C_SELECT, .render = c_select_render,
		.on_event = c_select_on_event };
	select->bg_color = 0xFF505050;
	select->fg_color = 0xFFFFFFFF;
	select->selected_item = -1;
	select->text = text;
	select->item_height = 30;
	return ((t_component *)select);
}
