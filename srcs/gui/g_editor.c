/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 17:13:30 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	
}

void	g_editor_on_enter(t_gui *self)
{

}

void	g_editor_on_leave(t_gui *self)
{

}

void	g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{

}

void	g_editor_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF0000);
}
