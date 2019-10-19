/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 12:50:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/19 17:37:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

// t_bool	alloc_components(t_gui *gui, int count)
// {
// 	if (!(gui->components = (t_component **)malloc(sizeof(t_component *) * count)))
// 		return (FALSE);
// 	ft_bzero(gui->components, sizeof(t_component *) * count);
// 	gui->component_count = count;
// 	return (TRUE);
// }

void	render_components(t_doom *doom, t_gui *gui)
{
	int	i;

	i = -1;
	while (++i < gui->components->len)
		gui->components->values[i]->render(doom, gui->components->values[i], &doom->screen);
}

void	free_components(t_gui *gui)
{
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
	while (i++ <= S_WIDTH - 5)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFFFF;
	i = 5;
	while (j++ <= 60)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFFFF;
	while (i++ <= S_WIDTH - 5)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFFFF;
	j = 5;
	while (j++ <= 60)
		doom->screen.pixels[j * doom->screen.width + i] = 0xFFFFFFFF;
	render_components(doom, self);
}
