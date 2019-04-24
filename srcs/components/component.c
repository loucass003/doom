/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 12:50:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 14:06:01 by llelievr         ###   ########.fr       */
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
