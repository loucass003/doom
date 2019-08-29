/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 20:50:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	render(t_doom *doom)
{
/*	ft_bzero(doom->rendered_area, doom->screen.width);
	traverseDrawOrder(doom, doom->bsp);*/
	if (doom->current_gui < 0)
		return ;
	const t_gui		*gui = &doom->guis[doom->current_gui];
	gui->render((t_gui *)gui, doom);
}
