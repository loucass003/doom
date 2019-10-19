#include <libft.h>
#include "gui.h"
#include "doom.h"

void		c_scrollbox_on_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	// t_select	*select;
	// t_vec2		pos;

	// select = (t_select *)self;
	// pos = (t_vec2){ event->motion.x, event->motion.y };
}

void		c_scrollbox_render(t_doom *doom, t_component *self, t_img *image)
{
	// t_select	*select;
	// int			i;

	// if (self->type != C_SCROLLBOX)
	// 	return ;
	// select = (t_select *)self;
	// fill_rect(image, self->bounds, select->bg_color);
}

t_component	 *create_scrollbox(SDL_Rect bounds)
{
	// t_select *select;

	// if (!(select = (t_select *)ft_memalloc(sizeof(t_select))))
	// 	return (NULL);
	// select->super = (t_component) { .bounds = bounds, .type = C_SCROLLBOX, 
	// 	.render = c_scrollbox_render, .on_event = c_scrollbox_on_event };
	// select->bg_color = 0xFF505050;
	// select->fg_color = 0xFFFFFFFF;
	// select->selected_item = -1;
	// select->text = text;
	// select->item_height = 30;
	// if (!(select->items = create_select_items_array(5)))
	// 	return (NULL);
	// return ((t_component *)select);
}