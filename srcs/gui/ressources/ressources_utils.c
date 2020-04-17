/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 12:23:55 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/17 21:07:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

void				values_selects(t_gui *self, t_ressource_manager *rm, int i)
{
	int				res_index;
	t_select		*s;
	t_textfield		*text;

	res_index = rm->page * PAGE_SIZE + i;
	if (!rm->ressources->values[res_index])
		return ;
	self->components->values[i * 3]->visible = TRUE;
	s = (t_select *)self->components->values[i * 3];
	s->selected_item = rm->ressources->values[res_index]->type - 1;
	self->components->values[i * 3 + 1]->visible = TRUE;
	text = (t_textfield *)self->components->values[i * 3 + 1];
	ft_strcpy(text->text,
	rm->ressources->values[res_index]->display_name);
	text->text_len = ft_strlen(rm->ressources->values[res_index]->display_name);
	text->super.enabled = !rm->ressources->values[res_index]->fixed;
	s->super.enabled = !rm->ressources->values[res_index]->fixed
		&& !rm->ressources->values[res_index]->loaded;
	s->fg_color = !s->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
	text->fg_color = !text->super.enabled ? 0xFFAAAAAA : 0xFFFFFFFF;
	self->components->values[i * 3 + 2]->visible =
		!rm->ressources->values[res_index]->fixed;
	self->components->values[i * 3 + 2]->enabled =
		!rm->ressources->values[res_index]->fixed;
}

void				update_selects(t_gui *self, t_ressource_manager *rm)
{
	int			i;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (rm->ressources->len - (rm->page * PAGE_SIZE)))
			values_selects(self, rm, i);
		else
		{
			self->components->values[i * 3]->visible = FALSE;
			self->components->values[i * 3 + 1]->visible = FALSE;
			self->components->values[i * 3 + 2]->visible = FALSE;
		}
	}
}

t_vec2				get_mouse_pos(t_doom *doom)
{
	int	x;
	int	y;
	int	x_w;
	int	y_w;

	SDL_GetWindowPosition(doom->win, &x_w, &y_w);
	SDL_GetGlobalMouseState(&x, &y);
	return (t_vec2){ x - x_w, y - y_w };
}

void				apply_select_value(t_gui *self, t_doom *doom)
{
	int			i;
	t_select	*select;
	t_textfield *tf;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		if (i < (doom->res_manager.ressources->len
			- (doom->res_manager.page * PAGE_SIZE)))
		{
			select = (t_select *)self->components->values[i * 3];
			if (select->selected_item >= 0)
				doom->res_manager.ressources->values[doom->res_manager.page
					* PAGE_SIZE + i]->type =
						select->items->values[select->selected_item].value;
			tf = (t_textfield *)self->components->values[i * 3 + 1];
			ft_strcpy(doom->res_manager.ressources
				->values[doom->res_manager.page
					* PAGE_SIZE + i]->display_name, tf->text);
		}
	}
}

int					get_pages_count(t_ressource_manager *rm)
{
	return (ceil((rm->ressources->len - 1) / PAGE_SIZE));
}
