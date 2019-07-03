/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 12:50:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 11:50:56 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	alloc_components(t_gui *gui, int count)
{
	if (!(gui->components = (t_component **)malloc(sizeof(t_component *) * count)))
		return (FALSE);
	ft_bzero(gui->components, sizeof(t_component *) * count);
	gui->component_count = count;
	return (TRUE);
}

void	render_components(t_doom *doom, t_gui *gui)
{
	int	i;

	i = -1;
	while (++i < gui->component_count)
		gui->components[i]->render(doom, gui->components[i]);
}

void	free_components(t_gui *gui)
{
	int	i;

	i = -1;
	while (++i < gui->component_count)
		free(gui->components[i]);
	free(gui->components);
}

t_bool	in_bounds(SDL_Rect bounds, t_vec2 pos)
{
	return (pos.x >= bounds.x && pos.y >= bounds.y 
		&& pos.y < bounds.y + bounds.h 
		&& pos.x < bounds.x + bounds.w);
}

void	display_comp(t_doom *doom, t_gui *self)
{
	int i;
	int j;
	i = 5;
	j = 5;
	while (i++ <= 1275)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFF;
	i = 5;
	while (j++ <= 44)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFF;
	while (i++ <= 1275)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFF;
	j = 5;
	while (j++ <= 44)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFF;
	render_components(doom, self);
}