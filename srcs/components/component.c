/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 12:50:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/05 16:35:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

void	render_components(t_doom *doom, t_gui *gui)
{
	int	i;

	i = gui->components->len;
	while (--i >= 0)
		if (gui->components->values[i]->visible)
			gui->components->values[i]->render(doom,
				gui->components->values[i], &doom->screen);
}

void	free_component(t_component **c_addr)
{
	t_component		*c;

	c = *c_addr;
	if (c->type == C_TEXTFIELD)
		free(((t_textfield *)c)->text);
	else if (c->type == C_SELECT && ((t_select *)c)->items
		&& ((t_select *)c)->items->auto_free)
		ft_memdel((void **)&((t_select *)c)->items);
	ft_memdel((void **)c_addr);
}

void	free_components(t_gui *gui)
{
	int		i;

	if (!gui->components)
		return ;
	i = -1;
	while (++i < gui->components->len)
		free_component(&gui->components->values[i]);
	free(gui->components);
	gui->components = NULL;
}

t_bool	in_bounds(SDL_Rect bounds, t_vec2 pos)
{
	return (pos.x >= bounds.x && pos.y >= bounds.y
		&& pos.y < bounds.y + bounds.h
		&& pos.x < bounds.x + bounds.w);
}

void	components_events(t_doom *doom, t_gui *guis, SDL_Event *event, int id)
{
	t_gui	*gui;
	int		i;

	if (id < 0)
		return ;
	gui = &guis[id];
	i = -1;
	while (++i < gui->components->len)
	{
		if (gui->components->values[i]->enabled
			&& gui->components->values[i]->on_event
			&& !gui->components->values[i]->on_event(gui->components->values[i],
				event, doom))
			break ;
	}
}
