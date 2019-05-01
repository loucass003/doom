/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 16:15:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	if (!(doom.rendered_area = (uint8_t *)malloc(doom.screen.width)))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	return (0);
}
